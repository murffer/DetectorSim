#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
   ~StackingAction();
     
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

