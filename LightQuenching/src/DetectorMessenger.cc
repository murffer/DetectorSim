#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger(
                                           DetectorConstruction* Det)
:Detector(Det)
{ 
  GS20LightYieldDir = new G4UIdirectory("/GS20LightYield/");
  GS20LightYieldDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/GS20LightYield/det/");
  detDir->SetGuidance("detector control");
    
  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/GS20LightYield/det/setGS20Thick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
  AbsThickCmd->SetParameterName("Size",false);
  AbsThickCmd->SetRange("Size>=0.");
  AbsThickCmd->SetUnitCategory("Length");
  AbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  RefThickCmd = new G4UIcmdWithADoubleAndUnit("/GS20LightYield/det/setRefThick",this);
  RefThickCmd->SetGuidance("Set Thickness of the Light Reflector (teflon)");
  RefThickCmd->SetParameterName("Size",false);
  RefThickCmd->SetRange("Size>=0.");
  RefThickCmd->SetUnitCategory("Length");
  RefThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  MntThickCmd = new G4UIcmdWithADoubleAndUnit("/GS20LightYield/det/setMntThick",this);
  MntThickCmd->SetGuidance("Set Thickness of the Mounting (Optical Grease)");
  MntThickCmd->SetParameterName("Size",false);
  MntThickCmd->SetRange("Size>=0.");
  MntThickCmd->SetUnitCategory("Length");
  MntThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeRadiusCmd = new G4UIcmdWithADoubleAndUnit("/GS20LightYield/det/setGS20Radius",this);
  SizeRadiusCmd->SetGuidance("Set tranverse size of the calorimeter");
  SizeRadiusCmd->SetParameterName("Size",false);
  SizeRadiusCmd->SetRange("Size>0.");
  SizeRadiusCmd->SetUnitCategory("Length");    
  SizeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/GS20LightYield/det/update",this);
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
  delete GS20LightYieldDir;  
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  
  if( command == AbsThickCmd )
   { Detector->SetGS20Thickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == MntThickCmd )
   { Detector->SetMountingThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == RefThickCmd )
   { Detector->SetReflectorThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == SizeRadiusCmd )
   { Detector->SetGS20Radius(SizeRadiusCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

