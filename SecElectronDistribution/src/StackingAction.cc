#include "StackingAction.hh"
#include "G4String.hh"
#include "G4VProcess.hh"


#include "G4Track.hh"


StackingAction::StackingAction()
{ }


StackingAction::~StackingAction()
{ }


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
    //keep up the 4th generation, kill secondaries
    if(track->GetParentID() == 1){
      const G4VProcess *p = track->GetCreatorProcess();
      if( !p)
        return fUrgent;
      else{
        G4String pName = track->GetCreatorProcess()->GetProcessName();
        if (pName == "NeutronInelastic" || pName == "compt")
          return fUrgent;
        else
          G4cout<<"Killed Proceses: "<<pName<<G4endl;
          return fKill;
      }
    }
    /*
    if (track->GetParentID() <= 4)    
        return fUrgent;
    else 
        return fKill;
    */
    else
      return fUrgent;
}
