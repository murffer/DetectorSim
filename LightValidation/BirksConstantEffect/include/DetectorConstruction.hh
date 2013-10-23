#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"

class G4Tubs;
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
    
		void SetBirksParameter(G4double);
		void UpdateGeometry();
		void PrintParameters();

private:

    /* METHODS */
		G4VPhysicalVolume* ConstructDetector();       /* Constructs the detector          */

    /* FIELDS */
		G4LogicalVolume*    worldLV;		// World
		G4VPhysicalVolume*  worldPV;
		G4VSolid*           worldS;

    G4LogicalVolume*    gs20LV;			/* GS20 Absorber */
		G4VPhysicalVolume*  gs20PV;			
		G4VSolid*	          gs20S;

		// Materials
		Materials* materials;            /* Material Manager                   */
    G4Material* absMaterial;        /* Absorber and Scintillator Material */

		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

