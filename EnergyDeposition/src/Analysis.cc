#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
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
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* ){
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
  G4double zPos = 0.0; 
  bool isFirst = true;
  CaloHit *hit;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();


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
        zPos -= hit->GetPosition().z();
      }

      // Adding the energy deposition (in MeV)
      eDepEvent += hit->GetEdep();
    }
  }
  // Adding to the run accumulation only events with deposit energy
  if (eDepEvent > 0.0){
    analysisManager->FillH1(1,eDepEvent);
    analysisManager->FillH2(1,eDepEvent/MeV,zPos/mm);
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* ){
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //save histograms
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}
/**
 * GetCalorimeterThickness
 * @return the thickness of the calorimeter
 */
G4double Analysis::GetCalorimeterThickness(){
  G4double caloThickness = 0;
  G4LogicalVolume* detLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Absorber");
  G4LogicalVolume* gapLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Gap");
  G4Tubs* detTubs = 0;
  G4Tubs* gapTubs = 0;
  if ( detLV && gapLV) {
    detTubs = dynamic_cast< G4Tubs*>(detLV->GetSolid());
    gapTubs = dynamic_cast< G4Tubs*>(gapLV->GetSolid());
  }
  if ( detTubs && gapTubs) {
    caloThickness = detTubs->GetZHalfLength()*2;
    caloThickness += gapTubs->GetZHalfLength()*1;
  }
  else  {
    G4cerr << "Calorimeter Thickness not found." << G4endl;
  } 
  return caloThickness;

}
