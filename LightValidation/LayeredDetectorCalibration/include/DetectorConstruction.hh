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

class AbsorberSD;
class PMTSD;

class DetectorConstruction : public G4VUserDetectorConstruction{

  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();                     /* Construct the detector         */
    G4VPhysicalVolume* ConstructVolumes();                      /* Construct the detector volumes */

    G4Material* FindMaterial(G4String);                                 /* Finds a Material     */
    G4double GetAbsThickness()		    {return detectorThickness;};      /* Detector Thickness   */
    G4Material* GetAbsorberMaterial()	{return absMaterial;};            /* Returns the Abosrber */

    void SetAbsorberThickness(G4double);
    void SetReflectorThickness(G4double);
    void SetMountingThickness(G4double);

    void UpdateGeometry();
    void PrintParameters();

  private:

    /* METHODS */
    G4VPhysicalVolume* ConstructDetector();                               /* Constructs the detector          */
    G4LogicalVolume* ConstructSource();                                   /* Constructs the source            */
    G4LogicalVolume* ConstructRPM8(std::list<G4double> const &slices);    /* Constructs the RPM8 Detector     */
    G4LogicalVolume* ConstructDetectorSlice(G4int);                       /* Constructs a detector slice      */
    G4LogicalVolume* ConstructReflector();                                /* Constructs the reflector         */

    void SetOpticalSurfaces();                    /* Sets up optical surfaces         */

    /* FIELDS */
    G4LogicalVolume*    worldLV;		// World
    G4VPhysicalVolume*  worldPV;
    G4VSolid*           worldS;

    std::vector<G4LogicalVolume*>   absLV; 			  /* Absorber */
    std::vector<G4VPhysicalVolume*> absPV;			
    std::vector<G4VSolid*>          absS;

    /*
       std::vector<G4LogicalVolume*>   absMountLV; 
       std::vector<G4VPhysicalVolume*> absMountPV;			
       std::vector<G4VSolid*>          absMountS;

       std::vector<G4LogicalVolume*>   mountLV;   
       std::vector<G4VPhysicalVolume*> mountPV;			
       std::vector<G4VSolid*>          mountS;
       */
    G4LogicalVolume*    sourceLV;     /* Source  */
    G4VPhysicalVolume*  sourcePV;
    G4VSolid*           sourceS;

    G4LogicalVolume*    rpm8LV;       /* RMP8 Box */
    G4VPhysicalVolume*  rpm8PV;
    G4VSolid*           rpm8S;

    G4LogicalVolume*    pmtTopLV;     /* Top PMT    */
    G4VPhysicalVolume*  pmtTopPV;
    G4LogicalVolume*    pmtBottomLV;  /* Bottom PMT */
    G4VPhysicalVolume*  pmtBottomPV;

    G4LogicalVolume*    refLV;        /* Light Reflector encasing entire detector */
    G4VPhysicalVolume*  refPV;
    G4VSolid*           refS;

    G4LogicalVolume*    mountLV;      /* PMT Mounting (Optical Grease) */
    G4VPhysicalVolume*  mountPV;
    G4VSolid*           mountS;

    // Materials
    Materials* materials;            /* Material Manager                   */
    G4Material* absMaterial;        /* Absorber and Scintillator Material */

    // Geometry parameters
    G4double rpm8Thickness;       /* Thickness of the RPM8 SAIC Detector      */
    G4double rpm8Width;           /* Width of the RPM8 SAIC Detector Module   */
    G4double rpm8Height;          /* Height of the RPM8 SAIC Detector Module  */
    G4double srcRadius;           /* Cf-252 Source Radius                     */
    G4double pbSrcThickness;      /* Pb thickness around Cf252 source         */
    G4double modSrcThickness;     /* HDPE moderator around Cf252 source       */
    G4double detectorThickness;   /* Thickness of a detector slice            */
    G4double pmtThickness;        /* Thickness of the PMT Glass               */
    G4double mountThickness;      /* Thickness of the optical grease          */
    G4double detMountThickness;   /* Thickness of the detector mount material */
    G4double refThickness;        /* Thickness of Reflector around the RPM8   */
    G4double pmtRadius;           /* Radius of the PMT used in the RPM8       */
    G4double lightGuideHeight;    /* Height of the light guide to the PMT     */
    G4double sliceThickness;      /* Thickness of a detector slice            */

    G4Material* detMaterial;        /* Detector Material (GS20 Glass)         */
    G4Material* mountMaterial;      /* Mounting Material (Optical Grease      */
    G4Material* detMountMaterial;   /* Detector Mounting Material (PMMA Dics  */
    G4Material* refMaterial;        /* Reflector Material (Teflon Tape)       */
    G4Material* pmtMaterial;        /* PMT Material (Boroscilate (BK7) Glass  */
    G4Material* lightGuideMaterial; /* Material for the light guide           */

    // Sensitive Detectors
    PMTSD*  pmtTopSD;               /* PMT SD  (Top PMT)        */
    PMTSD*  pmtBottomSD;            /* PMT SD  (Bottom PMT)     */
    PMTSD*  lgTopSD;                /* PMT SD  (Top Light Guide)*/
    PMTSD*  lgBotSD;                /* PMT SD  (Bottom LG)      */

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

