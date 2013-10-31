#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4SteppingManager.hh"
#include "G4VProcess.hh"

/**
 * Creates a hook into a class that is called at every step in the Geant4
 * simulation. Initilization of the fiels is preformed.
 *
 * @param run - the RunAction
 * @param event - the EventAction
 */
SteppingAction::SteppingAction(EventAction* event)
:fEventAction(event)
{ }

/**
 * Fills the step length and increments the energy depostion. Only the step
 * length of the primary particle (Track ID = 1) is booked in the histogram.
 * 
 * @param aStep - the step
 */
void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
   
  G4double EdepStep = aStep->GetTotalEnergyDeposit();
  if (EdepStep > 0.) { fEventAction->AddEdep(EdepStep);}


}
