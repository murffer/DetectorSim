#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"


#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

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
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }   
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 */
void Analysis::PrepareNewEvent(const G4Event* ){
  // Initialize energy deposition to zero
  nOPAbsEvent = 0.0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* ){
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
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
    G4cout
      << "\n\tAverage Number of Optical Photons: " 
      << analysisManager->GetH1(1)->mean() 
      << " +/- " << analysisManager->GetH1(1)->rms()
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
