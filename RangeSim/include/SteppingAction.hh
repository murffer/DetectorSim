#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class RunAction;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(RunAction*, EventAction*);
   ~SteppingAction() {};

    virtual void UserSteppingAction(const G4Step*);
    
  private:
    RunAction*    fRunAction;       /** Pointer to RunAction    */
    EventAction*  fEventAction;     /** Pointer to EventAction  */
};
#endif
