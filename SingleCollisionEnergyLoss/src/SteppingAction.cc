
#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

#include "G4TrackVector.hh"
#include <vector>

#include "Analysis.hh"
SteppingAction::SteppingAction() : G4UserSteppingAction(){ 
    // Nothing to be done
    killNextTrack = false;
}


SteppingAction::~SteppingAction() { }


void SteppingAction::UserSteppingAction(const G4Step* step){
   bool killNextTrack = Analysis::GetInstance()->GetKillStatus();
   if( killNextTrack){
        step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
    }
    
    // Want to kill certian processes    
   if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() 
        == "e-_G4DNAIonisation"){
        Analysis::GetInstance()->SetKillStatus(true);
    } 
}
