#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "G4Event.hh"

#include "PhotonHit.hh"
#include "AbsorberHit.hh"

#include "HistoManager.hh"

#ifdef USE_MPI
#include "G4MPImanager.hh"
#include <stdio.h>
#endif

Analysis* Analysis::singleton = 0;

/**
 * Analysis
 *
 * Creates an Analysis object 
 */
Analysis::Analysis(){ }
Analysis::~Analysis(){}
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
/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 * There is no need to update the geometry because the geometry
 * is fixed during a run.
 *
 * The files are named according to their rank for MPI Runs
 */
void Analysis::PrepareNewRun(const G4Run*){

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
 * Sets the number of optical photons generated
 */
void Analysis::SetNumOpticalPhotonsGenerated(G4int numPhotons){
  nOPAbsEvent = numPhotons;
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
   * The PMT SD has a single hit collection associated with it, while the
   * absorber SD has two hit collections assciated with it.
   */
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  G4int pmtHCID = fSDM->GetCollectionID("PMTHitCollection");
  G4int absHCID = fSDM->GetCollectionID("AbsHitCollection");
  G4HCofThisEvent* HCofEvent = event->GetHCofThisEvent();
 
  AbsHitsCollection* absHC = (AbsHitsCollection*) (HCofEvent->GetHC(absHCID));
  PhotonHitsCollection* pmtHC = (PhotonHitsCollection*) (HCofEvent->GetHC(pmtHCID));

  // The Number of Optical Photons Detected on the PMT
  if(pmtHC != NULL)
    nOPPMTEvent += pmtHC->GetSize();
  else
    G4cout<<"WARNING: Absorber::EndOfEvent - pmtHC is NULL"<<G4endl;
  
  // Average Energy Deposition and Optical Photons Generated
  AbsorberHit *absHit;
  for (unsigned int i = 0; i < absHC->GetSize(); i++){
    absHit =  (AbsorberHit*) absHC->GetHit(i);
      eDepEvent += absHit->GetEdep();
  }
    
  // Filling the histograms
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (eDepEvent > 0.0){
      analysisManager->FillH1(1,eDepEvent);
      analysisManager->FillH1(2,nOPAbsEvent);
      analysisManager->FillH1(3,nOPPMTEvent);
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* ){
    // Print some output
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if ( analysisManager->IsActive() ) {
			G4cout<<"\tAverage Energy Deposited: "
			<< G4BestUnit(analysisManager->GetH1(1)->mean(),"Energy")
			<< " +/- " << G4BestUnit(analysisManager->GetH1(1)->rms(),"Energy")
			<< "\n\tAverage Number of Optical Photons Created: "
			<< analysisManager->GetH1(2)->mean()
			<< " +/- " << analysisManager->GetH1(2)->rms()
			<< "\n\tAverage Number of Optical Photons Detected: "
			<< analysisManager->GetH1(3)->mean()
			<< " +/- " << analysisManager->GetH1(3)->rms()
			<< G4endl;
   	} 
    //save histograms
    if ( analysisManager->IsActive() ) {
        analysisManager->Write();
        analysisManager->CloseFile();
    }
}
