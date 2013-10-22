#include "DetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
 : Detector(Det)
{
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance(" Geometry Setup ");

  UpdateCmd = new G4UIcmdWithoutParameter("/det/Update",this);
  UpdateCmd->SetGuidance("Update musr geometry");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  SetPhotonDetPolishCmd = new G4UIcmdWithADouble("/det/setPhotonDetPolish", this);
  SetPhotonDetPolishCmd->SetGuidance("Set the polish of the mirror");
  SetPhotonDetPolishCmd->SetParameterName("polish",false);
  SetPhotonDetPolishCmd->SetRange("polish>0 && polish<=1");
  SetPhotonDetPolishCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetPhotonDetReflectivityCmd = new G4UIcmdWithADouble("/det/setPhotonDetReflectivity", this);
  SetPhotonDetReflectivityCmd->SetGuidance("Set the reflectivity of the mirror");
  SetPhotonDetReflectivityCmd->SetParameterName("reflectivity",false);
  SetPhotonDetReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  SetPhotonDetReflectivityCmd->AvailableForStates(G4State_PreInit);
    
    SetScintThicknessCmd = new G4UIcmdWithADouble("/det/setPhotonDetReflectivity", this);
    SetScintThicknessCmd->SetGuidance("Set the reflectivity of the mirror");
    SetScintThicknessCmd->SetParameterName("thickness",false);
    SetScintThicknessCmd->SetRange("thickness>=0");
    SetScintThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    SetScintMaterialCmd = new G4UIcmdWithAString("/det/setSlabMaterial",this);
    SetScintMaterialCmd->SetGuidance("Select Material of the Scintillating Slab.");
    SetScintMaterialCmd->SetParameterName("choice",false);
    SetScintMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    SetPMTMaterialCmd = new G4UIcmdWithAString("/det/setPMTMaterial",this);
    SetPMTMaterialCmd->SetGuidance("Select Material of the PMT.");
    SetPMTMaterialCmd->SetParameterName("choice",false);
    SetPMTMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete detDir;
  delete UpdateCmd;
    delete SetPhotonDetPolishCmd;
    delete SetPhotonDetReflectivityCmd;
    delete SetPMTMaterialCmd;
    delete SetScintMaterialCmd;

}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String val)
{
  if( command == UpdateCmd ) {
    Detector->UpdateGeometry();
  }
   else if( command == SetPhotonDetPolishCmd ) {
    Detector->SetPhotonDetPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetPhotonDetReflectivityCmd ) {
    Detector-> SetPhotonDetReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetScintThicknessCmd ) {
      Detector-> SetScintThickness(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetScintMaterialCmd ) {
      Detector-> SetScintMaterial(val);
  }
  else if( command == SetPMTMaterialCmd ) {
      Detector-> SetPMTMaterial(val);
  }

}
