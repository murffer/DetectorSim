#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "DetectorConstruction.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithoutParameter;

class DetectorMessenger : public G4UImessenger
{
  public:

    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();
 
    void SetNewValue(G4UIcommand*, G4String);

  private:

    DetectorConstruction*   Detector;
 
    G4UIdirectory*          detDir;

    G4UIcmdWithoutParameter*   UpdateCmd;

    G4UIcmdWithAString*        SetPhotonDetGeometryCmd;
    G4UIcmdWithAnInteger*      SetNumOfCladLayersCmd;
    G4UIcmdWithADoubleAndUnit* SetLengthCmd;
    G4UIcmdWithADoubleAndUnit* SetRadiusCmd;
    G4UIcmdWithADoubleAndUnit* SetClad1RadiusCmd;
    G4UIcmdWithADoubleAndUnit* SetClad2RadiusCmd;
    G4UIcmdWithADoubleAndUnit* SetPhotonDetHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* SetGapCmd;
    G4UIcmdWithADoubleAndUnit* SetPhotonDetAlignmentCmd;
    G4UIcmdWithADouble*        SetXYRatioCmd;
    G4UIcmdWithADouble*        SetSurfaceRoughnessCmd;
    G4UIcmdWithADouble*        SetMirrorPolishCmd;
    G4UIcmdWithADouble*        SetMirrorReflectivityCmd;
    G4UIcmdWithADouble*        SetPhotonDetPolishCmd;
    G4UIcmdWithADouble*        SetPhotonDetReflectivityCmd;
    G4UIcmdWithABool*          SetMirrorCmd;
    G4UIcmdWithADoubleAndUnit* SetBarLengthCmd;
    G4UIcmdWithADoubleAndUnit* SetBarBaseCmd;
    G4UIcmdWithADoubleAndUnit* SetHoleRadiusCmd;
    G4UIcmdWithADoubleAndUnit* SetCoatingThicknessCmd;
    G4UIcmdWithADoubleAndUnit* SetCoatingRadiusCmd;

};

#endif
