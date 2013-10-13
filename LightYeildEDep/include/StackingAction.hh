
#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"


class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
    virtual ~StackingAction();
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        
    virtual void PrepareNewEvent();
    virtual void NewStage();

  private:
    G4int optPhotonCounter;     /* Number of optical photon's created per event     */
};


#endif
