#include "AnalysisMessenger.hh"

#include "Analysis.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

AnalysisMessenger::AnalysisMessenger(){ 
  /* Command Directory */
  AnalysisDir = new G4UIdirectory("/Analysis/");
  AnalysisDir->SetGuidance("Analysis Commands");
 
  /* Particle Filename */
  ParticleNameCmd = new G4UIcmdWithAString("/Analysis/setParticleName",this);
  ParticleNameCmd->SetGuidance("Set the incident particle name");
  ParticleNameCmd->SetParameterName("choice",false);
  ParticleNameCmd->SetCandidates("neutron Co60");
  ParticleNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

AnalysisMessenger::~AnalysisMessenger(){
  delete ParticleNameCmd;
  delete AnalysisDir;  
}

void AnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue){ 
  if( command == ParticleNameCmd ){
    Analysis::GetInstance()->SetIncidentParticleName(newValue);
  }
}

