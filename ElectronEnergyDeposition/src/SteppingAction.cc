#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4VProcess.hh"

/**
 * Creates a hook into a class that is called at every step in the Geant4
 * simulation. Initilization of the fiels is preformed.
 *
 * @param run - the RunAction
 * @param event - the EventAction
 */
SteppingAction::SteppingAction(RunAction* run, EventAction* event)
:fRunAction(run), fEventAction(event)
{ }

/**
 * Fills the step length and increments the energy depostion. Only the step
 * length of the primary particle (Track ID = 1) is booked in the histogram.
 * 
 * @param aStep - the step
 */
void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   
  G4double EdepStep = aStep->GetTotalEnergyDeposit();
  if (EdepStep > 0.) {  fRunAction->AddEdep(EdepStep);
                      fEventAction->AddEdep(EdepStep);
  }


}
