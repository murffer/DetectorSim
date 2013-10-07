#include "StackingAction.hh"
#include "HistoManager.hh"

#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* track)
{  
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //
  //energy spectrum of secondaries
  //
  G4double energy = track->GetKineticEnergy();
  G4double charge = track->GetDefinition()->GetPDGCharge();

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  if (charge != 0.) analysisManager->FillH1(5,energy);
  else              analysisManager->FillH1(6,energy);   
  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
