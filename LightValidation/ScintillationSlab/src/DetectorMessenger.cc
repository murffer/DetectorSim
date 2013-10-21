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

  SetPhotonDetGeometryCmd =
                      new G4UIcmdWithAString("//setPhotonDetGeometry",this);
  SetPhotonDetGeometryCmd->
                  SetGuidance("Select the geometry of the PhotonDet detector");
  SetPhotonDetGeometryCmd->SetGuidance("Only Accepts 'Circle' and 'Square'");
  SetPhotonDetGeometryCmd->SetCandidates("Circle Square");
  SetPhotonDetGeometryCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  SetNumOfCladLayersCmd = new G4UIcmdWithAnInteger("//setNumOfLayers", this);
  SetNumOfCladLayersCmd->SetGuidance("Select the number of cladding layers");
  SetNumOfCladLayersCmd->SetGuidance("Maximum number is 2");
  SetNumOfCladLayersCmd->SetParameterName("numberOfLayers",false);
  SetNumOfCladLayersCmd->SetRange("numberOfLayers>=0 && numberOfLayers<=2");
  SetNumOfCladLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetSurfaceRoughnessCmd =
                      new G4UIcmdWithADouble("//setSurfaceRoughness", this);
  SetSurfaceRoughnessCmd->
                  SetGuidance("Set the roughness between Clad1 and  Fiber");
  SetSurfaceRoughnessCmd->SetParameterName("roughness",false);
  SetSurfaceRoughnessCmd->SetRange("roughness>0 && roughness<=1");
  SetSurfaceRoughnessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetXYRatioCmd = new G4UIcmdWithADouble("//setXYRatio", this);
  SetXYRatioCmd->SetGuidance("Set the ratio between x and y axis (x/y)");
  SetXYRatioCmd->SetParameterName("ratio",false);
  SetXYRatioCmd->SetRange("ratio>0 && ratio<=1");
  SetXYRatioCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetMirrorPolishCmd = new G4UIcmdWithADouble("//setMirrorPolish", this);
  SetMirrorPolishCmd->SetGuidance("Set the polish of the mirror");
  SetMirrorPolishCmd->SetParameterName("polish",false);
  SetMirrorPolishCmd->SetRange("polish>0 && polish<=1");
  SetMirrorPolishCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetMirrorReflectivityCmd =
                    new G4UIcmdWithADouble("//setMirrorReflectivity", this);
  SetMirrorReflectivityCmd->SetGuidance("Set the reflectivity of the mirror");
  SetMirrorReflectivityCmd->SetParameterName("reflectivity",false);
  SetMirrorReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  SetMirrorReflectivityCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

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

  SetLengthCmd = new G4UIcmdWithADoubleAndUnit("//setLength",this);
  SetLengthCmd->SetGuidance("Set the half length of the  fiber");
  SetLengthCmd->SetParameterName("length",false);
  SetLengthCmd->SetRange("length>0.");
  SetLengthCmd->SetUnitCategory("Length");
  SetLengthCmd->SetDefaultUnit("mm");
  SetLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetRadiusCmd = new G4UIcmdWithADoubleAndUnit("//setRadius",this);
  SetRadiusCmd->SetGuidance("Set the radius of the  fiber");
  SetRadiusCmd->SetParameterName("radius",false);
  SetRadiusCmd->SetRange("radius>0.");
  SetRadiusCmd->SetUnitCategory("Length");
  SetRadiusCmd->SetDefaultUnit("mm");
  SetRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetClad1RadiusCmd = new G4UIcmdWithADoubleAndUnit("//setClad1Radius",this);
  SetClad1RadiusCmd->SetGuidance("Set the radius of Cladding 1");
  SetClad1RadiusCmd->SetParameterName("radius",false);
  SetClad1RadiusCmd->SetRange("radius>0.");
  SetClad1RadiusCmd->SetUnitCategory("Length");
  SetClad1RadiusCmd->SetDefaultUnit("mm");
  SetClad1RadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetClad2RadiusCmd = new G4UIcmdWithADoubleAndUnit("//setClad2Radius",this);
  SetClad2RadiusCmd->SetGuidance("Set the radius of Cladding 2");
  SetClad2RadiusCmd->SetParameterName("radius",false);
  SetClad2RadiusCmd->SetRange("radius>0.");
  SetClad2RadiusCmd->SetUnitCategory("Length");
  SetClad2RadiusCmd->SetDefaultUnit("mm");
  SetClad2RadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetPhotonDetHalfLengthCmd =
             new G4UIcmdWithADoubleAndUnit("//setPhotonDetHalfLength",this);
  SetPhotonDetHalfLengthCmd->
                      SetGuidance("Set the half length of PhotonDet detector");
  SetPhotonDetHalfLengthCmd->SetParameterName("halfL",false);
  SetPhotonDetHalfLengthCmd->SetRange("halfL>0.");
  SetPhotonDetHalfLengthCmd->SetUnitCategory("Length");
  SetPhotonDetHalfLengthCmd->SetDefaultUnit("mm");
  SetPhotonDetHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetGapCmd = new G4UIcmdWithADoubleAndUnit("//setGap",this);
  SetGapCmd->SetGuidance("Set the distance between PhotonDet and fiber end");
  SetGapCmd->SetParameterName("theta",false);
  SetGapCmd->SetUnitCategory("Length");
  SetGapCmd->SetDefaultUnit("mm");
  SetGapCmd->SetRange("theta>=0.");
  SetGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetPhotonDetAlignmentCmd =
                       new G4UIcmdWithADoubleAndUnit("//setAlignment",this);
  SetPhotonDetAlignmentCmd->
                     SetGuidance("Set the deviation of PhotonDet from z axis");
  SetPhotonDetAlignmentCmd->SetParameterName("theta",false);
  SetPhotonDetAlignmentCmd->SetUnitCategory("Angle");
  SetPhotonDetAlignmentCmd->SetDefaultUnit("deg");
  SetPhotonDetAlignmentCmd->SetRange("theta>-90. && theta<90.");
  SetPhotonDetAlignmentCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetMirrorCmd = new G4UIcmdWithABool("//setMirror", this);
  SetMirrorCmd->SetGuidance("Place a mirror at the end of the fiber");
  SetMirrorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetBarLengthCmd = new G4UIcmdWithADoubleAndUnit("//setBarLength",this);
  SetBarLengthCmd->SetGuidance("Set the length of the scintillator bar");
  SetBarLengthCmd->SetParameterName("length",false);
  SetBarLengthCmd->SetRange("length>0.");
  SetBarLengthCmd->SetUnitCategory("Length");
  SetBarLengthCmd->SetDefaultUnit("mm");
  SetBarLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetBarBaseCmd = new G4UIcmdWithADoubleAndUnit("//setBarBase",this);
  SetBarBaseCmd->SetGuidance("Set the side length of the scintillator bar");
  SetBarBaseCmd->SetParameterName("length",false);
  SetBarBaseCmd->SetRange("length>0.");
  SetBarBaseCmd->SetUnitCategory("Length");
  SetBarBaseCmd->SetDefaultUnit("mm");
  SetBarBaseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetHoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("//setHoleRadius",this);
  SetHoleRadiusCmd->SetGuidance("Set the radius of the fiber hole");
  SetHoleRadiusCmd->SetParameterName("radius",false);
  SetHoleRadiusCmd->SetRange("radius>0.");
  SetHoleRadiusCmd->SetUnitCategory("Length");
  SetHoleRadiusCmd->SetDefaultUnit("mm");
  SetHoleRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetCoatingThicknessCmd =
               new G4UIcmdWithADoubleAndUnit("//setCoatingThickness",this);
  SetCoatingThicknessCmd->
                   SetGuidance("Set thickness of the coating on the bars");
  SetCoatingThicknessCmd->SetParameterName("thick",false);
  SetCoatingThicknessCmd->SetUnitCategory("Length");
  SetCoatingThicknessCmd->SetDefaultUnit("mm");
  SetCoatingThicknessCmd->SetRange("thick>=0.");
  SetCoatingThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SetCoatingRadiusCmd =
                new G4UIcmdWithADoubleAndUnit("//setCoatingRadius",this);
  SetCoatingRadiusCmd->
                    SetGuidance("Set inner radius of the corner bar coating");
  SetCoatingRadiusCmd->SetParameterName("cradius",false);
  SetCoatingRadiusCmd->SetUnitCategory("Length");
  SetCoatingRadiusCmd->SetDefaultUnit("mm");
  SetCoatingRadiusCmd->SetRange("cradius>=0.");
  SetCoatingRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

DetectorMessenger::~DetectorMessenger()
{
  delete detDir;

  delete UpdateCmd;

  delete SetPhotonDetGeometryCmd;
  delete SetNumOfCladLayersCmd;
  delete SetLengthCmd;
  delete SetRadiusCmd;
  delete SetClad1RadiusCmd;
  delete SetClad2RadiusCmd;
  delete SetPhotonDetHalfLengthCmd;
  delete SetGapCmd;
  delete SetPhotonDetAlignmentCmd;
  delete SetSurfaceRoughnessCmd;
  delete SetMirrorPolishCmd;
  delete SetMirrorReflectivityCmd;
  delete SetXYRatioCmd;
  delete SetMirrorCmd;
  delete SetBarLengthCmd;
  delete SetBarBaseCmd;
  delete SetHoleRadiusCmd;
  delete SetCoatingThicknessCmd;
  delete SetCoatingRadiusCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String val)
{
  if( command == UpdateCmd ) {

    Detector->UpdateGeometry();
  }
  else if( command == SetPhotonDetGeometryCmd ) {
 
    Detector->SetPhotonDetGeometry(val);
  }
  else if( command == SetNumOfCladLayersCmd ) {

    Detector->SetNumberOfCladding(G4UIcmdWithAnInteger::GetNewIntValue(val));
  }
  else if( command == SetSurfaceRoughnessCmd ) {

    Detector->SetSurfaceRoughness(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetXYRatioCmd ) {
 
    Detector->SetXYRatio(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetMirrorPolishCmd ) {

    Detector->SetMirrorPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetMirrorReflectivityCmd ) {
 
    Detector->
             SetMirrorReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetPhotonDetPolishCmd ) {
 
    Detector->SetPhotonDetPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetPhotonDetReflectivityCmd ) {
 
    Detector->
          SetPhotonDetReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == SetLengthCmd ) {
 
    Detector->SetLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetRadiusCmd ) {
 
    Detector->SetRadius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetClad1RadiusCmd ) {
 
    Detector->
             SetClad1Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetClad2RadiusCmd ) {
 
    Detector->
             SetClad2Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetPhotonDetHalfLengthCmd ) {
 
    Detector->
     SetPhotonDetHalfLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetGapCmd ) {
 
   Detector->SetGap(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetPhotonDetAlignmentCmd ) {
 
   Detector->
      SetPhotonDetAlignment(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == SetMirrorCmd ) {

   Detector->SetMirror(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == SetBarLengthCmd ) {

   Detector->SetBarLength(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == SetBarBaseCmd ) {

   Detector->SetBarBase(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == SetHoleRadiusCmd ) {

   Detector->SetHoleRadius(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == SetCoatingThicknessCmd ) {

   Detector->SetCoatingThickness(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == SetCoatingRadiusCmd ) {

   Detector->SetCoatingRadius(G4UIcmdWithABool::GetNewBoolValue(val));
  }
}
