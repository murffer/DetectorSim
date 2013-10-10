#include "AnalysisMessenger.hh"

#include "Analysis.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

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
  
  /* Histogram Boundries */
  HistEMaxCmd = new G4UIcmdWithADoubleAndUnit("/Analysis/setHistogramMax",this);
  HistEMaxCmd->SetGuidance("Set Maximum Energy of the Analysis Histogram");
  HistEMaxCmd->SetParameterName("Size",false);
  HistEMaxCmd->SetRange("Size>=0.");
  HistEMaxCmd->SetUnitCategory("Energy");
  HistEMaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
      
  HistBinWidthCmd = new G4UIcmdWithADoubleAndUnit("/Analysis/setPosBinWidth",this);
  HistBinWidthCmd->SetGuidance("Set width of the pos bin histogram");
  HistBinWidthCmd->SetParameterName("Size",false);
  HistBinWidthCmd->SetRange("Size>=0.");
  HistBinWidthCmd->SetUnitCategory("Length");
  HistBinWidthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  /* Energy Deposition Position Dependance */
  EDepPosCmd = new G4UIcmdWithABool("/Analysis/setEDepPosAnalysis",this);
  EDepPosCmd->SetGuidance("Analyze Energy Deposition and Position");
  EDepPosCmd->SetParameterName("setEDepPosAnalysis",false);
  EDepPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

AnalysisMessenger::~AnalysisMessenger(){
  delete ParticleNameCmd;
  delete HistEMaxCmd; 
  delete HistBinWidthCmd;
  delete EDepPosCmd;
  delete AnalysisDir;  
}

void AnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue){ 
  if( command == ParticleNameCmd ){
    Analysis::GetInstance()->SetIncidentParticleName(newValue);
  }
   
  if( command == HistEMaxCmd ){
    G4double value = HistEMaxCmd->GetNewDoubleValue(newValue);
    Analysis::GetInstance()->SetHistEMax(value);
  }

  if( command == HistBinWidthCmd){
    G4double value = HistEMaxCmd->GetNewDoubleValue(newValue);
    Analysis::GetInstance()->SetBinWidth(value);
  }
  if( command == EDepPosCmd){
    G4bool value = EDepPosCmd->GetNewBoolValue(newValue);
    Analysis::GetInstance()->SetEDepPosAnalysis(value);
  }
}

