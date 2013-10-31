#include "EventAction.hh"

#include "EventActionMessenger.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

/**
 * Creates an event hook for each event.
 *
 * The default print is set to every 100,000 events along without drawing 
 * and using the default EventActionMessenger.
 */
EventAction::EventAction()
  :fDrawFlag("none"),fPrintModulo(100000),fEventMessenger(0)
{
  fEventMessenger = new EventActionMessenger(this);
}
/**
 * Deconstructor by deleting the event messenger
 */
EventAction::~EventAction()
{
  delete fEventMessenger;
}
/**
 * Initilizes the energy deposition of the event and prints the event
 * if the event is the modulus of the fPrintModulo
 *
 * @param evt - the event
 */
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
/**
 * Fills the energy deposition at the end of the event. The histogram is filled
 * using the G4AnalysisManagerInstance, as setup in RunAction.
 *
 * @param evt - the event
 */
void EventAction::EndOfEventAction(const G4Event*)
{
  G4AnalysisManager::Instance()->FillH1(1,fTotalEnergyDeposit);
}
