#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:fDetector(Det)
{ 
  fTestemDir = new G4UIdirectory("/lightQuench/");
  fTestemDir->SetGuidance("commands specific to this example");
  
  fDetDir = new G4UIdirectory("/lightQuench/det/");
  fDetDir->SetGuidance("detector construction commands");
        
  fMaterCmd = new G4UIcmdWithAString("/lightQuench/det/setMat",this);
  fMaterCmd->SetGuidance("Select material of the box.");
  fMaterCmd->SetParameterName("choice",false);
  fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  fUpdateCmd = new G4UIcmdWithoutParameter("/lightQuench/det/update",this);
  fUpdateCmd->SetGuidance("Update calorimeter geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd;
  delete fUpdateCmd;
  delete fDetDir;
  delete fTestemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fMaterCmd )
   { fDetector->SetMaterial(newValue);}
   
  if( command == fUpdateCmd )
   { fDetector->UpdateGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
