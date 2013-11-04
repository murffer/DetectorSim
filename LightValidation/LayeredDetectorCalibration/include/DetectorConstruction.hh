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

class PMTSD;

class DetectorConstruction : public G4VUserDetectorConstruction{
    
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();                     /* Construct the detector         */
    G4VPhysicalVolume* ConstructVolumes();                      /* Construct the detector volumes */
    
    G4Material* FindMaterial(G4String);                                 /* Finds a Material     */
   
		void SetDetectorMaterial(G4String);
    void SetAbsorberThickness(G4double);
    void SetReflectorThickness(G4double);
    void SetMountingThickness(G4double);
    
    void UpdateGeometry();
    void PrintParameters();
    
private:
    
    /* METHODS */
    G4VPhysicalVolume* ConstructDetector();                               /* Constructs the detector          */
    G4LogicalVolume* ConstructPMMA(std::list<G4double> const &slices);    /* Constructs the PMMA Detector     */
    G4LogicalVolume* ConstructDetectorSlice(G4int);                       /* Constructs a detector slice      */
    G4LogicalVolume* ConstructReflector();                                /* Constructs the reflector         */
    
    void SetOpticalSurfaces();                    /* Sets up optical surfaces         */
    
    /* FIELDS */
    G4LogicalVolume*    worldLV;		// World
    G4VPhysicalVolume*  worldPV;
    G4VSolid*           worldS;
    
    G4LogicalVolume*    cladLV;     /* Cladding */
    G4VPhysicalVolume*  cladPV;
    G4VSolid*           cladS;
    
    std::vector<G4LogicalVolume*>   absLV; 			  /* Absorber */
    std::vector<G4VPhysicalVolume*> absPV;
    std::vector<G4VSolid*>          absS;
    
    G4LogicalVolume*    pmmaLV;       /* PMMA Slabs */
    G4VPhysicalVolume*  pmmaPV;
    G4VSolid*           pmmaS;
    
    G4LogicalVolume*    pmtLV;     /* Top PMT    */
    G4VPhysicalVolume*  pmtPV;
    
    G4LogicalVolume*    refLV;        /* Light Reflector encasing entire detector */
    G4VPhysicalVolume*  refPV;
    G4VSolid*           refS;
    
    G4LogicalVolume*    mountPMTLV;      /* PMT Mounting (Optical Grease) */
    G4VPhysicalVolume*  mountPMTPV;
    G4VSolid*           mountPMTS;
    
    // Materials
    Materials* materials;           /* Material Manager                   */
    G4Material* absMaterial;        /* Absorber and Scintillator Material */
    G4Material* cladMaterial;       /* Reflector and cladding 								*/
    G4Material* detMaterial;        /* Detector Material (GS20 Glass)         */
    G4Material* mountMaterial;      /* Mounting Material (Optical Grease      */
    G4Material* pmtMaterial;        /* PMT Material (Boroscilate (BK7) Glass  */
    G4Material* lightGuideMaterial; /* Material for the light guide           */
    
    // Geometry parameters
    G4double pmmaThickness;         /* Thickness of the encsasing PMMA     */
    G4double pmmaWidth;           /* Width of the encasing PMMA   */
    G4double pmmaHeight;          /* Height of the encasing PMMA  */

    G4double detectorThickness;   /* Thickness of a detector slice            */
    G4double pmtThickness;        /* Thickness of the PMT Glass               */
    G4double mountThickness;      /* Thickness of the optical grease          */
    G4double refThickness;        /* Thickness of Reflector around the RPM8   */
    G4double pmtRadius;           /* Radius of the PMT used in the RPM8       */
    G4double lightGuideHeight;    /* Height of the light guide to the PMT     */
    G4double sliceThickness;      /* Thickness of a detector slice  (with mounting)          */
    
    
    // Sensitive Detectors
    PMTSD*  pmtSD;               /* PMT SD  (Top PMT)        */
  
    
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

