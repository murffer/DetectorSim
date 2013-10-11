#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* Detector;
    
    G4UIdirectory*             GS20LightYieldDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithADoubleAndUnit* AbsThickCmd;
    G4UIcmdWithADoubleAndUnit* MntThickCmd;
    G4UIcmdWithADoubleAndUnit* RefThickCmd;
    G4UIcmdWithADoubleAndUnit* SizeRadiusCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;
};

#endif

