#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "HistoManager.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#ifdef G4MPIUSE
#include "G4MPImanager.hh"
#endif

Analysis* Analysis::singleton = 0;

/**
 * Analysis
 *
 * Creates an Analysis object 
 */
Analysis::Analysis(){ 
  fHistoManager = new HistoManager();
}
/**
 * Allocates the memory for storing the voxel positions
 */
void Analysis::SetupVoxelPositions(){
  // Setting up the positions
  xPos = new G4double [numVoxels];
  yPos = new G4double [numVoxels];
  for (G4int i = 0; i < numVoxels; i++){
    xPos[i] = 0;
    yPos[i] = 0;
  }

}
/**
 * Sets the x, y position of the voxel number
 *
 * @param voxNum - voxel number
 * @param x  - x position of the voxel
 * @param y - y positon of the voxel
 */
void Analysis::SetVoxelPosition(G4int voxNum, G4double x, G4double y){
  xPos[voxNum] = x;
  yPos[voxNum] = y;
}
/**
 * Nothing to do!
 */
Analysis::~Analysis(){
  delete fHistoManager; 
  delete xPos;
  delete yPos;
  delete eDepEvent;
  delete eDepEvent2;
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 */
void Analysis::PrepareNewRun(const G4Run* aRun){
  G4AnalysisManager *man  = G4AnalysisManager::Instance();
  if (man->IsActive()){
    man->OpenFile();
  }
  G4cout<<"Prepared run "<<aRun->GetRunID()<<G4endl;
  // Initialize energy deposition to zero
  eDepEvent = new G4double [numVoxels];
  eDepEvent2 = new G4double [numVoxels];
  for (G4int i = 0; i < numVoxels; i++){
    eDepEvent[i] = 0;
    eDepEvent2[i] = 0;
  }
  eDepEventTotal = 0;
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* anEvent){
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  G4double eDep;
  G4int copyNum;
  G4VHitsCollection *hc;
  G4int chamberNum;
  CaloHit *hit;
  
  // Iterating through the hit collection 
  G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
  for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
   
    hc = event->GetHCofThisEvent()->GetHC(hitItter);
    for(G4int i = 0; i < hc->GetSize(); i++){
      hit = (CaloHit*) hc->GetHit(i);
      eDep = hit->GetEdep()/MeV;
      copyNum = hit->GetChamberNumber();
      eDepEvent[copyNum] += eDep;
      eDepEvent2[copyNum] += eDep*eDep;

      // Adding the energy deposition (in MeV)
      eDepEventTotal += hit->GetEdep()/MeV;
    }
  }
  // Adding to the run accumulation 
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->FillH1(1,eDepEventTotal);
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
  
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  G4double n = double(NbOfEvents);

  // Outputing the data
  for(G4int i = 0; i < numVoxels; i++){
    // Computing the mean and rms
    G4double mean = eDepEvent[i]/n;
    G4double mean2 = eDepEvent2[i]/n;
    G4double rms = mean2 - mean*mean;
    if (rms > 0. ) rms = std::sqrt(rms); else rms = 0.;
    G4cout<<i<<"\t"
      <<G4BestUnit(mean,"Energy")<<" +/- "<<G4BestUnit(rms,"Energy")
      <<G4endl;
  }
  
  // Save Histograms
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  if (man->IsActive()){
    man->Write();
    man->CloseFile();
  }
}
