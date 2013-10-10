
#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
:fPhysicsList(pPhys)
{ 
  fPhysDir = new G4UIdirectory("/energy/phys/");
  fPhysDir->SetGuidance("physics list commands");

  fGammaCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setGCut",this);  
  fGammaCutCmd->SetGuidance("Set gamma cut.");
  fGammaCutCmd->SetParameterName("Gcut",false);
  fGammaCutCmd->SetUnitCategory("Length");
  fGammaCutCmd->SetRange("Gcut>0.0");
  fGammaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fElectCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setECut",this);  
  fElectCutCmd->SetGuidance("Set electron cut.");
  fElectCutCmd->SetParameterName("Ecut",false);
  fElectCutCmd->SetUnitCategory("Length");
  fElectCutCmd->SetRange("Ecut>0.0");
  fElectCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fProtoCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setPCut",this);  
  fProtoCutCmd->SetGuidance("Set positron cut.");
  fProtoCutCmd->SetParameterName("Pcut",false);
  fProtoCutCmd->SetUnitCategory("Length");
  fProtoCutCmd->SetRange("Pcut>0.0");
  fProtoCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
  
  fAlphaCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setACut",this);  
  fAlphaCutCmd->SetGuidance("Set alpha cut.");
  fAlphaCutCmd->SetParameterName("Acut",false);
  fAlphaCutCmd->SetUnitCategory("Length");
  fAlphaCutCmd->SetRange("Acut>0.0");
  fAlphaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fTritonCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setTCut",this);  
  fTritonCutCmd->SetGuidance("Set triton cut.");
  fTritonCutCmd->SetParameterName("Tcut",false);
  fTritonCutCmd->SetUnitCategory("Length");
  fTritonCutCmd->SetRange("Tcut>0.0");
  fTritonCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAllCutCmd = new G4UIcmdWithADoubleAndUnit("/energy/phys/setCuts",this);  
  fAllCutCmd->SetGuidance("Set cut for all.");
  fAllCutCmd->SetParameterName("cut",false);
  fAllCutCmd->SetUnitCategory("Length");
  fAllCutCmd->SetRange("cut>0.0");
  fAllCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
  delete fGammaCutCmd;
  delete fTritonCutCmd;
  delete fAlphaCutCmd;
  delete fElectCutCmd;
  delete fProtoCutCmd;
  delete fAllCutCmd;
  delete fPhysDir;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{       
  if( command == fGammaCutCmd )
   { fPhysicsList->SetCutForGamma(fGammaCutCmd->GetNewDoubleValue(newValue));}
     
  if( command == fElectCutCmd )
   { fPhysicsList->SetCutForElectron(fElectCutCmd->GetNewDoubleValue(newValue));}
  
  if( command == fAlphaCutCmd )
   { fPhysicsList->SetCutForAlpha(fAlphaCutCmd->GetNewDoubleValue(newValue));}
  
  if( command == fTritonCutCmd )
   { fPhysicsList->SetCutForTriton(fTritonCutCmd->GetNewDoubleValue(newValue));}
     
  if( command == fProtoCutCmd )
   { fPhysicsList->SetCutForPositron(fProtoCutCmd->GetNewDoubleValue(newValue));}

  if( command == fAllCutCmd )
    {
      G4double cut = fAllCutCmd->GetNewDoubleValue(newValue);
      fPhysicsList->SetCutForGamma(cut);
      fPhysicsList->SetCutForElectron(cut);
      fPhysicsList->SetCutForPositron(cut);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
