#include "PrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                             PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  fGunDir = new G4UIdirectory("/testem/gun/");
  fGunDir->SetGuidance("gun control");
 
  fDefaultCmd = new G4UIcmdWithAnInteger("/testem/gun/setDefault",this);
  fDefaultCmd->SetGuidance("set/reset kinematic defined in PrimaryGenerator");
  fDefaultCmd->SetGuidance("0=boxCenter, else=frontFace");
  fDefaultCmd->SetParameterName("position",true);
  fDefaultCmd->SetDefaultValue(1);
  fDefaultCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fRndmCmd = new G4UIcmdWithADouble("/testem/gun/rndm",this);
  fRndmCmd->SetGuidance("random lateral extension on the beam");
  fRndmCmd->SetGuidance("in fraction of 0.5*sizeYZ");
  fRndmCmd->SetParameterName("rBeam",false);
  fRndmCmd->SetRange("rBeam>=0.&&rBeam<=1.");
  fRndmCmd->AvailableForStates(G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fDefaultCmd;
  delete fRndmCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == fDefaultCmd)
   {Action->SetDefaultKinematic(fDefaultCmd->GetNewIntValue(newValue));}
   
  if (command == fRndmCmd)
   {Action->SetRndmBeam(fRndmCmd->GetNewDoubleValue(newValue));}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

