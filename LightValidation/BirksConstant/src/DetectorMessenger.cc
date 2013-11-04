#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction*  Det)
:Detector(Det)
{ 

    
  DetThickCmd = new G4UIcmdWithADoubleAndUnit("/det/setDetThick",this);
  DetThickCmd->SetGuidance("Set Thickness of the Absorber");
  DetThickCmd->SetParameterName("Size",false);
  DetThickCmd->SetRange("Size>=0.");
  DetThickCmd->SetUnitCategory("Length");
  DetThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  DetMatCmd = new G4UIcmdWithAString("/det/setDetMaterial",this);
  DetMatCmd->SetGuidance("Set the detector material");
  DetMatCmd->SetCandidates("GS20 PSLiF EJ426");
  DetMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  BirksCmd = new G4UIcmdWithADouble("/det/setBirks",this);
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
    delete DetThickCmd;
    delete DetMatCmd;
    delete UpdateCmd;
    delete detDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  
  if( command == DetThickCmd )
   { Detector->SetDetectorThickness(DetThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == DetMatCmd )
  { Detector->SetDetectorMaterial(newValue);}
   
  if( command == BirksCmd )
   { Detector->SetBirksConstant(BirksCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

