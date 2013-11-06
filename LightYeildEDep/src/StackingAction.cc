#include "StackingAction.hh"
#include "G4String.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "HistoManager.hh"
#include "Analysis.hh"

StackingAction::StackingAction(){
   optPhotonCounter = 0;
}


StackingAction::~StackingAction()
{ }
/**
 * NewStage
 *
 * Seems to be called once stack is empty, so only after an event is over
 */
void StackingAction::NewStage(){

    // Updating Analysis
    Analysis::GetInstance()->SetNumOpticalPhotonsGenerated(optPhotonCounter);
}
/**
 * PrepareNewEvent
 *
 * Prepares a new event for the stacking action.
 * The only action we need to do is reset the counters
 */
void StackingAction::PrepareNewEvent(){
  optPhotonCounter = 0;
}
/**
 * ClassifyNewTrack
 *
 * Called every time a new track needs to be classified, so we can use it to
 * determine in the new track is an optical photon
 */
G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track){
  // Optical Photon Counter
  if(track->GetDefinition()== G4OpticalPhoton::OpticalPhotonDefinition()){
      optPhotonCounter++;
			return fKill;
  }
  return fUrgent;
}
