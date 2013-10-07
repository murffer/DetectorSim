//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "EventActionMessenger.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:fDrawFlag("none"),fPrintModulo(10000),fEventMessenger(0)
{
  fEventMessenger = new EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
 G4int evtNb = evt->GetEventID();
 
 //printing survey
 if (evtNb%fPrintModulo == 0) {
    G4cout << "\n---> Begin of Event: " << evtNb << G4endl;
}
 
 //additional initializations            
 fTotalEnergyDeposit = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{

  G4AnalysisManager::Instance()->FillH1(4,fTotalEnergyDeposit);
  
  ////if (fDrawFlag != "none") G4cout << " Energy deposit: "
  ////                               << G4BestUnit(fTotalEnergyDeposit,"Energy")
  ////                               << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


