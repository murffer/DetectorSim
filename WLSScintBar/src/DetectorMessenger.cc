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
    
    
    SetScintThicknessCmd = new G4UIcmdWithADouble("/det/setScintThickness", this);
    SetScintThicknessCmd->SetGuidance("Set the thickness of the slab scintillator");
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
    
    SetWLSMaterialCmd = new G4UIcmdWithAString("/det/setWLSMaterial",this);
    SetWLSMaterialCmd->SetGuidance("Select Material of the Wavelength Shifter.");
    SetWLSMaterialCmd->SetParameterName("choice",false);
    SetWLSMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    delete detDir;
    delete UpdateCmd;
    delete SetPMTMaterialCmd;
    delete SetScintMaterialCmd;
    delete SetScintThicknessCmd;
    delete SetWLSMaterialCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String val)
{
    if( command == UpdateCmd ) {
        Detector->UpdateGeometry();
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
    else if( command == SetWLSMaterialCmd ) {
        Detector-> SetWLSMaterial(val);
    }
    
}
