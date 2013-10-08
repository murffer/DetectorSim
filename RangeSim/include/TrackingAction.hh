#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"

class PrimaryGeneratorAction;
class RunAction;
class G4GeneralParticleSource;

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(PrimaryGeneratorAction*, RunAction*);
    ~TrackingAction() {};

    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    PrimaryGeneratorAction* fPrimary;   /** Primary Generator Action  */
    RunAction*              fRunAction; /** RunAction                 */
};
#endif
