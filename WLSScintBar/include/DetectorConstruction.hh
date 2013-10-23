#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4RotationMatrix.hh"

class G4Box;
class G4Tubs;
class G4EllipticalTube;

class G4LogicalVolume;
class G4VPhysicalVolume;

class Materials;
class G4Material;

class DetectorMessenger;

class PhotonDetSD;

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    DetectorConstruction();
    ~DetectorConstruction();
    
    G4VPhysicalVolume* Construct();
    G4VPhysicalVolume* ConstructDetector();
    
    void UpdateGeometry();
    void PrintParameters();
    
    // Set Material Commands for World and Scintillator
    void SetWorldMaterial        (G4String);
    void SetWLSMaterial          (G4String);
    void SetScintMaterial        (G4String);
    void SetPMTMaterial          (G4String);
    
    void SetPhotonDetPolish     (G4double);
    void SetPhotonDetReflectivity (G4double);
    void SetScintThickness      (G4double);
    
    
    G4Material* FindMaterial(G4String);
    
private:
    
    Materials* materials;
    G4Material* detMaterial;
    G4Material* pmtMaterial;
    G4Material* wlsMaterial;
    
    G4Box*              solidWorld;
    G4LogicalVolume*    logicWorld;
    G4VPhysicalVolume*  physiWorld;
    
    G4Box*              solidScintillator;
    G4LogicalVolume*    logicScintillator;
    G4VPhysicalVolume*  physiScintillator;
    
    G4Box*              solidPhotonDet;
    G4LogicalVolume*    logicPhotonDet;
    G4VPhysicalVolume*  physiPhotonDet;
    
    G4Box*              solidWLS;
    G4LogicalVolume*    logicWLS;
    G4VPhysicalVolume*  physiWLS;
    
    G4double           worldSizeX;
    G4double           worldSizeY;
    G4double           worldSizeZ;
    
    G4double           scintX;
    G4double           scintY;
    G4double           scintZ;
    
    G4double            wlsThickness;
    G4double            pmtLength;
    
    G4double pmtPolish;
    G4double pmtReflectivity;
    
    static PhotonDetSD* pmtSD;
    
private:
    
    void ConstructFiber();
    
    void UpdateGeometryParameters();
    
    DetectorMessenger* detectorMessenger;
    
};

#endif
