#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* Det):Detector(Det)
{ 
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance("detector control");
 
	BirksCmd = new G4UIcmdWithADouble("/det/setBirks",this);
  BirksCmd->SetGuidance("Set Birks constant of detector");
  BirksCmd->SetParameterName("birks",false);
  BirksCmd->SetRange("birks>0.");
  BirksCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
}

DetectorMessenger::~DetectorMessenger()
{
  delete BirksCmd;
	delete UpdateCmd;
  delete detDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  
	if( command == BirksCmd )
   { Detector->SetBirksParameter(BirksCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

