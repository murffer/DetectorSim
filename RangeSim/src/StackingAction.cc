#include "StackingAction.hh"
#include "HistoManager.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"

/**
 * Default Constructor - nothing to be done
 */
StackingAction::StackingAction(){ }
/**
 * Deconstructor
 */
StackingAction::~StackingAction(){ }
/**
 * Classifies a new track, and fills the energy of the charged and neutral
 * particles of the secondaries.
 */
G4ClassificationOfNewTrack 
StackingAction::ClassifyNewTrack(const G4Track* track)
{  
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //
  //energy spectrum of secondaries
  G4double energy = track->GetKineticEnergy();
  G4double charge = track->GetDefinition()->GetPDGCharge();

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  if (charge != 0.){
    analysisManager->FillH1(5,energy);
/*
    G4ParticleDefinition* p = track->GetDefinition();
    // Only filling for electrons
    if (p->GetPDGEncoding() == 11){
      G4String procName = track->GetCreatorProcess()->GetProcessName();
      if(procName == "hIoni")
        analysisManager->FillH1(7,energy);
      if(procName == "compt")
        analysisManager->FillH1(8,energy);
      //analysisManager->FillH1(5,energy);
    }
 */
  }
  else              analysisManager->FillH1(6,energy);   
//  return fKill;
  return fUrgent;
}
