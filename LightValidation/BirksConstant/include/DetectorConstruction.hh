#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include <list>
#include <vector>
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"

class G4Box;
class G4Tubs;
class G4UnionSolid;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;
class Materials;


class DetectorConstruction : public G4VUserDetectorConstruction{
    
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();                     /* Construct the detector         */
    G4VPhysicalVolume* ConstructVolumes();                      /* Construct the detector volumes */
    
    G4Material* FindMaterial(G4String);                                 /* Finds a Material     */
    
    void SetDetectorThickness(G4double);
    void SetBirksConstant(G4double);
    void SetDetectorMaterial(G4String);
    
    void UpdateGeometry();
    void PrintParameters();
    
private:
    
    /* METHODS */
    G4VPhysicalVolume* ConstructDetector();                               /* Constructs the detector          */
    void SetOpticalSurfaces();                    /* Sets up optical surfaces         */
    
    /* FIELDS */
    G4LogicalVolume*    worldLV;		// World
    G4VPhysicalVolume*  worldPV;
    G4VSolid*           worldS;
    
    G4LogicalVolume*    detLV;     /* Detector */
    G4VPhysicalVolume*  detPV;
    G4VSolid*           detS;
    
    // Materials
    Materials* materials;           /* Material Manager                   */
    G4Material* detMaterial;        /* Absorber and Scintillator Material */

    G4double detectorThickness;   /* Thickness of a detector slice            */
  

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

