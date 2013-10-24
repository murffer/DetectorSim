#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"

#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#ifdef USE_MPI
#include "G4MPImanager.hh"
#include <stdio.h>
#endif

#include "HistoManager.hh"
Analysis* Analysis::singleton = 0;
/**
 * Analysis
 *
 * Creates an Analysis object
 */
Analysis::Analysis(){
}
/**
 * Initilize the analysis manager
 *
 * Creates a new HistoManager
 */
void Analysis::Initilize(){
  fHistoManager = new HistoManager();
}
/**
 * Cleaning up the analysis manager
 *
 * Deleting the HistoManager
 */
void Analysis::CleanUp(){
  delete fHistoManager;
}
Analysis::~Analysis(){
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 * There is no need to update the geometry because the geometry
 * is fixed during a run.
 */
void Analysis::PrepareNewRun(const G4Run* ){
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
#ifdef USE_MPI
	if ( analysisManager->IsActive() ) {
				// Creating a filename based on the rank
				G4int rank = G4MPImanager::GetManager()-> GetRank();
				char str[64];
				sprintf(str, "-%03d", rank);
				G4String fname = analysisManager->GetFileName() + G4String(str);
        analysisManager->OpenFile(fname);
  }  
#else
	if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }  
#endif
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* ){
  // Initialize energy deposition to zero
  nOPDetEvent = 0.0;
  nOPAbsEvent = 0.0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){
  G4VHitsCollection *hc;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Iterating through the hit collection to accumulate the energy deposition 
  G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
  for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
    // Itterating through the hit collection
    hc = event->GetHCofThisEvent()->GetHC(hitItter);
    nOPDetEvent += hc->GetSize();

  }
  // Adding to the run accumulation only events with deposit energy
  if (nOPAbsEvent > 0.0){
    analysisManager->FillH1(2,nOPDetEvent);
  analysisManager->FillH1(1,nOPAbsEvent);
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* ){
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Print some output
    G4cout<<"\tAverage Number of Optical Photons Created: " 
      << analysisManager->GetH1(1)->mean()
      << " +/- " << analysisManager->GetH1(1)->rms()
      << "\n\tAverage Number of Optical Photons Detected: " 
      << analysisManager->GetH1(2)->mean() 
      << " +/- " << analysisManager->GetH1(2)->rms()
      << G4endl;
  
  //save histograms
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}
/**
 * Sets the number of optical photons generated
 */
void Analysis::SetNumOpticalPhotonsGenerated(G4int numPhotons){
  nOPAbsEvent = numPhotons;
}
