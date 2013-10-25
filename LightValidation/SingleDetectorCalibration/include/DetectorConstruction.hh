#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

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
		G4double GetDetectorThickness()		    {return detThickness;};  /* Detector Thickness       */
		G4double GetDetectorRadius()			    {return detRadius;};     /* Detector Radius          */
		G4Material* GetAbsorberMaterial()	{return absMaterial;};    /* Returns the Abosrber */
    
		void SetDetectorThickness(G4double);
    void SetReflectorThickness(G4double);
    void SetMountingThickness(G4double);
    void SetDetectorRadius(G4double);
		void SetDetectorMaterial(G4String);

		void UpdateGeometry();
		void PrintParameters();

private:

    /* METHODS */
		G4VPhysicalVolume* ConstructDetector();       /* Constructs the detector          */
		void SetVisAttributes();                      /* Sets visulationation attributes  */
		void SetSensitiveDetectors();                 /* Sets up sensitive detectors      */
    void SetOpticalSurfaces();                    /* Sets up optical surfaces         */

    /* FIELDS */
		G4LogicalVolume*    worldLV;		// World
		G4VPhysicalVolume*  worldPV;
		G4VSolid*           worldS;

    G4LogicalVolume*    gs20LV;			/* Detector Absorber */
		G4VPhysicalVolume*  gs20PV;			
		G4VSolid*	          gs20S;

    G4LogicalVolume*    pmtLV;      /* PMT */
    G4VPhysicalVolume*  pmtPV;
    G4VSolid*           pmtS;

    G4LogicalVolume*    pmtCapLV;      /* PMT Cap*/
    G4VPhysicalVolume*  pmtCapPV;
    G4VSolid*           pmtCapS;

    G4LogicalVolume*    pmtAirLV;      /* PMT Air*/
    G4VPhysicalVolume*  pmtAirPV;
    G4VSolid*           pmtAirS;

    G4LogicalVolume*    refLV;      /* LIght Reflector */
    G4VPhysicalVolume*  refPV;
    G4VSolid*           refS;

    G4LogicalVolume*    mountLV;      /* Mounting (Optical Grease) */
    G4VPhysicalVolume*  mountPV;
    G4VSolid*           mountS;
    
		// Materials
		Materials* materials;            /* Material Manager                   */
    G4Material* absMaterial;        /* Absorber and Scintillator Material */

		// Geometry parameters
    G4double detThickness;	      	// Thickness of Absorber
		G4double refThickness;        	// Thickness of Reflector
		G4double detRadius;		    	  // Outer Radius of Detector
    G4double pmtRadius;             // Radius of the PMT
		G4double detectorThickness;         // Thickness of entire detectorrimeter
    G4double pmtThickness;          /* Thickness of the PMT Glass       */
    G4double mountThickness;       /* Thickness of the optical grease */
    G4double capThickness;          /* Thickness of the cap    */
		G4double worldSizeXY;
		G4double worldSizeZ;

    G4Material* detMaterial;        /* Detector Material (GS20 Glass)         */
	  G4Material* mountMaterial;      /* Mounting Material (Optical Grease      */
    G4Material* refMaterial;        /* Reflector Material (Teflon Tape)       */
    G4Material* pmtMaterial;        /* PMT Material (Boroscilate (BK7) Glass  */

    // Sensitive Detectors
		AbsorberSD* absSD;              /* Absorber (GS20 Glass SD) */
    PMTSD*  pmtSD;                  /* PMT SD                   */

		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

