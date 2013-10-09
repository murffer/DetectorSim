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
Analysis::Analysis(){ }
/**
 * Nothing to do!
 */
Analysis::~Analysis(){
  G4cout<<"Deleting the analysis object"<<G4endl;
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 */
void Analysis::PrepareNewRun(const G4Run* aRun){
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
        // First interaction of the particle
        isFirst = false;
        xPos = hit->GetPosition().x();
        yPos = hit->GetPosition().y();
        zPos = hit->GetPosition().z();

      // Adding the energy deposition (in MeV)
      eDepEvent += hit->GetEdep()/MeV;
    }
  }
  // Adding to the run accumulation only events with deposit energy
  if (eDepEvent > 0.0){
      G4AnalysisManager* man = G4AnalysisManager::Instance();
      man->FillH1(1,eDepEvent);
     man->FillNtupleDColumn(0, );
     man->FillNtupleDColumn(1, fEnergyGap);
     man->AddNtupleRow();
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
}
