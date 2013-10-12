#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"

#include "HistoManager.hh"

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
void Analysis::PrepareNewRun(const G4Run* aRun){
  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }   
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event. Sets the energy deposited per event to be
 * zero, as well as the number of optical photons that are created and detected
 */
void Analysis::PrepareNewEvent(const G4Event*){
  eDepEvent = 0.0;
  nOPAbsEvent = 0;
  nOPPMTEvent = 0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  /**
   * Getting the hit collections. 
   */
 

  
  // Filling the histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(1,nOPAbsEvent);
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* ){
  // Print out some run statistics
  
  //save histograms      
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
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

