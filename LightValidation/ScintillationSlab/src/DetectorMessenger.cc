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
  detDir = new G4UIdirectory("//");
  detDir->SetGuidance(" Geometry Setup ");

  UpdateCmd = new G4UIcmdWithoutParameter("//Update",this);
  UpdateCmd->SetGuidance("Update musr geometry");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);



  SetPhotonDetPolishCmd =
                       new G4UIcmdWithADouble("//setPhotonDetPolish", this);
  SetPhotonDetPolishCmd->SetGuidance("Set the polish of the mirror");
  SetPhotonDetPolishCmd->SetParameterName("polish",false);
  SetPhotonDetPolishCmd->SetRange("polish>0 && polish<=1");
  SetPhotonDetPolishCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetPhotonDetReflectivityCmd =
                 new G4UIcmdWithADouble("//setPhotonDetReflectivity", this);
  SetPhotonDetReflectivityCmd->
                             SetGuidance("Set the reflectivity of the mirror");
  SetPhotonDetReflectivityCmd->SetParameterName("reflectivity",false);
  SetPhotonDetReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  SetPhotonDetReflectivityCmd->AvailableForStates(G4State_PreInit);
}

DetectorMessenger::~DetectorMessenger()
{
  delete detDir;

  delete UpdateCmd;


    delete SetPhotonDetPolishCmd;
    delete SetPhotonDetReflectivityCmd;

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
 
    Detector->
          SetPhotonDetReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
}
