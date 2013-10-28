#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger(
                                           DetectorConstruction* Det)
:Detector(Det)
{ 
  
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance("detector control");
    
  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/det/setDetectorThick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
  AbsThickCmd->SetParameterName("Size",false);
  AbsThickCmd->SetRange("Size>=0.");
  AbsThickCmd->SetUnitCategory("Length");
  AbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  RefThickCmd = new G4UIcmdWithADoubleAndUnit("/det/setRefThick",this);
  RefThickCmd->SetGuidance("Set Thickness of the Light Reflector (teflon)");
  RefThickCmd->SetParameterName("Size",false);
  RefThickCmd->SetRange("Size>=0.");
  RefThickCmd->SetUnitCategory("Length");
  RefThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  MntThickCmd = new G4UIcmdWithADoubleAndUnit("/det/setMntThick",this);
  MntThickCmd->SetGuidance("Set Thickness of the Mounting (Optical Grease)");
  MntThickCmd->SetParameterName("Size",false);
  MntThickCmd->SetRange("Size>=0.");
  MntThickCmd->SetUnitCategory("Length");
  MntThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeRadiusCmd = new G4UIcmdWithADoubleAndUnit("/det/setDetectorRadius",this);
  SizeRadiusCmd->SetGuidance("Set tranverse size of the calorimeter");
  SizeRadiusCmd->SetParameterName("Size",false);
  SizeRadiusCmd->SetRange("Size>0.");
  SizeRadiusCmd->SetUnitCategory("Length");    
  SizeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	DetMaterialCmd = new G4UIcmdWithAString("/det/setDetectorMaterial",this);
	DetMaterialCmd->SetGuidance("Set the detector material");
	DetMaterialCmd->SetCandidates("GS20 EJ426 PSLiF G4_PLASTIC_SC_VINYLTOLUENE");
  DetMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
}

DetectorMessenger::~DetectorMessenger()
{
	delete AbsThickCmd;
  delete RefThickCmd;
  delete MntThickCmd;
  delete SizeRadiusCmd; 
	delete UpdateCmd;
  delete detDir;
	delete DetMaterialCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  
  if( command == AbsThickCmd )
   { Detector->SetDetectorThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == MntThickCmd )
   { Detector->SetMountingThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == RefThickCmd )
   { Detector->SetReflectorThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == SizeRadiusCmd )
   { Detector->SetDetectorRadius(SizeRadiusCmd->GetNewDoubleValue(newValue));}
  
	if( command == DetMaterialCmd )
   { Detector->SetDetectorMaterial(newValue);}
		   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

