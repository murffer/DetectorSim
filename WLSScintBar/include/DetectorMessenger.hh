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

    G4UIcmdWithADouble*        SetScintThicknessCmd;
    G4UIcmdWithAString*        SetPMTMaterialCmd;
    G4UIcmdWithAString*        SetScintMaterialCmd;
    G4UIcmdWithAString*        SetWLSMaterialCmd;
    G4UIcmdWithAString*        SetCladMaterialCmd;

};

#endif
