#include "TrackingAction.hh"
#include "G4RichTrajectory.hh"
#include "G4TrackingManager.hh"

TrackingAction::TrackingAction() : G4UserTrackingAction() {
    // Nothing to be done in constructor
}

/*
 * PreUserTrackingAction
 *
 * Setting the trajectory to be a rich trajectory to hold additional info.
 */
void TrackingAction::PreUserTrackingAction(const G4Track* aTrack){
    fpTrackingManager->SetStoreTrajectory(true);
    fpTrackingManager->SetTrajectory(new G4RichTrajectory(aTrack));
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack) {}
