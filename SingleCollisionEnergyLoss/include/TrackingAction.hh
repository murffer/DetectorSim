/**
 * @file    TrackingAction.hh
 *
 * @date    15 Aug 2012
 * @author  murffer@utk.edu
 *
 * @brief   User's Tracking Action
 */
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
class TrackingAction : public G4UserTrackingAction {

    // Constructor and Deconstructor
    public:
        TrackingAction();
        virtual ~TrackingAction() {};

        // Member Functions
        virtual void PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);
};
#endif


