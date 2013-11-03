#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

/**
 * Creates an event hook for each event.
 *
 * The default print is set to every 100,000 events along without drawing
 * and using the default EventActionMessenger.
 */
EventAction::EventAction()
{
}
/**
 * Deconstructor
 */
EventAction::~EventAction(){
    
}

void EventAction::AddEdep(G4double xPos, G4double Edep){
    Analysis* analysis = Analysis::GetInstance();
    analysis->AddEDepEvent(xPos, Edep);
}
/**
 * Initilizes the energy deposition of the event and prints the event
 * if the event is the modulus of the fPrintModulo
 *
 * @param evt - the event
 */
void EventAction::BeginOfEventAction(const G4Event* anEvent){
    Analysis* analysis = Analysis::GetInstance();
    analysis->PrepareNewEvent(anEvent);
}
/**
 * Fills the energy deposition at the end of the event. The histogram is filled
 * using the G4AnalysisManagerInstance, as setup in RunAction.
 *
 * @param evt - the event
 */
void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    Analysis* analysis = Analysis::GetInstance();
    analysis->EndOfEvent(anEvent);
}
