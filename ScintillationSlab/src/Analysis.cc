#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"


#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#include "HistoManager.hh"
#include "PhotonDetHit.hh"
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
 * Sets the number of optical photons
 */
void Analysis::PrepareNewEvent(const G4Event* ){
    nOPAbsEvent = 0;
    nOPPMTEvent = 0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){
    G4VHitsCollection *hc;
    PhotonDetHit *hit;
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // Iterating through the hit collection to accumulate the energy deposition
    G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
    for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
        // Itterating through the hit collection
        hc = event->GetHCofThisEvent()->GetHC(hitItter);
        for(unsigned int i = 0; i < hc->GetSize(); i++){
            hit = (PhotonDetHit*) hc->GetHit(i);
            analysisManager->FillH1(1, hit->GetArrivalTime());
        }
    }
    
    // The Number of Optical Photons Detected on the PMT
    if(hc != NULL)
        nOPPMTEvent += hc->GetSize();
    else
        G4cout<<"WARNING: Absorber::EndOfEvent - pmtHC is NULL"<<G4endl;
    
    // Filling the histograms
    analysisManager->FillH1(2,nOPAbsEvent);
    analysisManager->FillH1(3,nOPPMTEvent);
    
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
    << "\n\tAverage Number of Optical Photons Created: "
    << analysisManager->GetH1(2)->mean()
    << " +/- " << analysisManager->GetH1(2)->rms()
    << "\n\tAverage Number of Optical Photons Detected: "
    << analysisManager->GetH1(3)->mean()
    << " +/- " << analysisManager->GetH1(3)->rms()
    << "\n\tAverage Time: "
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
