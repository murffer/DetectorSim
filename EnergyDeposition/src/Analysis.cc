#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"

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
  // Empty Constructor, assingment done in constuctor list
  maxHistEnergy = 5*MeV;
  posHistBinWidth = 5*um;
}
Analysis::~Analysis(){
  G4cout<<"Deleting the analysis object"<<G4endl;
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 * There is no need to update the geometry because the geometry
 * is fixed during a run.
 */
void Analysis::PrepareNewRun(const G4Run* aRun){

  
  // Creating ROOT analysis objects (histogram)
  outfile = new TFile(fname.data(),"RECREATE");
  eDepHist = new TH1F("eDepHist","Total Energy Deposition",100,0*eV,maxHistEnergy);
  eDepHist->GetXaxis()->SetTitle("Deposited Energy (MeV)");
  if (EDepPosAnalysis){
    posEDepTuple = new TNtuple("posEDepTuple","Initial Position and Energy Deposition","x:y:z:EnergyDep");
    posEDepHist = new TH2F("eDepPosHist","Positional Energy Dependance",25,0*eV,maxHistEnergy,25,0,detThickness/um);
    posEDepHist->GetXaxis()->SetTitle("Deposited Energy (MeV)");
    posEDepHist->GetYaxis()->SetTitle("First Interaction Position (um)");

  }
  G4cout<<"Prepared run "<<aRun->GetRunID()<<G4endl;
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* anEvent){
  // Initialize energy deposition to zero
  eDepEvent = 0.0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  G4VHitsCollection *hc;
  G4double xPos = 0.0; 
  G4double yPos = 0.0;
  G4double zPos = 0.0; 
  bool isFirst = true;
  CaloHit *hit;
 
  // Iterating through the hit collection to accumulate the energy deposition 
  G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
  for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
    // Itterating through the hit collection
   
    hc = event->GetHCofThisEvent()->GetHC(hitItter);
    for(G4int i = 0; i < hc->GetSize(); i++){
      hit = (CaloHit*) hc->GetHit(i);
      if (hit->GetTrackID() == 2 && hit->GetParentID() == 1 && isFirst){
        // First interaction of the particle
        isFirst = false;
        zPos = GetCalorimeterThickness(); // Subtracting the thickness
        xPos = hit->GetPosition().x();
        yPos = hit->GetPosition().y();
        zPos -= hit->GetPosition().z();
      }

      // Adding the energy deposition (in MeV)
      eDepEvent += hit->GetEdep()/MeV;
    }
  }
  // Adding to the run accumulation only events with deposit energy
  if (eDepEvent > 0.0){
    eDepHist->Fill(eDepEvent);
    if (EDepPosAnalysis){
      posEDepTuple->Fill(xPos,yPos,zPos,eDepEvent);
      posEDepHist->Fill(eDepEvent,zPos/um);    
    }
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){

  // Lets get the number of events simulated
  G4int numEvents = aRun->GetNumberOfEventToBeProcessed();
  G4double detThickness = GetDetectorThickness();
  
  G4cout<<"End Of Run "<<aRun->GetRunID()
        <<"\n\tRan "<<numEvents<<" events"
        <<"\n\tThickness [mm]: "<<detThickness/mm
        <<"\n\tAverage Energy Deposition per event (MeV): "<<eDepHist->GetMean()
        <<" +/- "<<eDepHist->GetMeanError()<<G4endl;
  outfile->Write();
  outfile->Close();
  delete outfile;
}
