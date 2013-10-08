#include "StackingAction.hh"

#include "G4Track.hh"


StackingAction::StackingAction()
{ }


StackingAction::~StackingAction()
{ }


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
    //keep primary particle, kill secondaries
    if (track->GetParentID() == 0)
        return fUrgent;
    else {
        return fKill;
    }

}

