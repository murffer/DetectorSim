#include "TrackingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Track.hh"
#include "G4GeneralParticleSource.hh"
/**
 * Creates a hook into the tracking action
 */
  TrackingAction::TrackingAction(PrimaryGeneratorAction* prim, RunAction* run)
:fPrimary(prim), fRunAction(run)
{ }

/**
 * Nothing is completed
 */
void TrackingAction::PreUserTrackingAction(const G4Track*)
{ }
/**
 * Computes the true and projected range. The track length is computed, and the
 * number of steps for the track are incremented in the histograms through 
 * G4AnalysisManager.
 *
 * @param aTrack - the track
 */
void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{  
  //increase nb of processed tracks 
  //count nb of steps of this track
  G4int   nbSteps = aTrack->GetCurrentStepNumber();
  G4double Trleng = aTrack->GetTrackLength();

  if (aTrack->GetDefinition()->GetPDGCharge() == 0.) {
    fRunAction->CountTraks0(1); 
    fRunAction->CountSteps0(nbSteps);

  } else {
    fRunAction->CountTraks1(1); 
    fRunAction->CountSteps1(nbSteps);
  }

  //true and projected ranges for primary particle
  if (aTrack->GetTrackID() == 1) {
    fRunAction->AddTrueRange(Trleng);
    G4ThreeVector vertex = fPrimary->GetParticleGun()->GetParticlePosition();    
    G4ThreeVector position = aTrack->GetPosition() - vertex;      
    fRunAction->AddProjRange(position.x());
    fRunAction->AddTransvDev(position.y());
    fRunAction->AddTransvDev(position.z());

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(1,Trleng);
    analysisManager->FillH1(2,(float)nbSteps);        
  }        
}
