#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"

class G4Box;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction{

	public:
		DetectorConstruction();
		virtual ~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();

		void PrintCaloParameters();

		G4double GetWorldSizeXY()			{return worldSizeXY;};
		G4double GetWorldSizeZ()			{return worldSizeZ;};
		G4double GetCaloThickness()		{return caloThickness;};
		G4double GetCaloRadius()			{return oRadius;};

		G4int GetNumberOfLayers()			{return nofLayers;};

		G4Material* GetAbsorberMaterial()		{return absMaterial;};
		G4double	 GetAbsorberThickness()			{return absThickness;};

		G4Material* GetGapMaterial()				{return gapMaterial;};
		G4double	 GetGapThickness()				{return gapThickness;};

		const G4VPhysicalVolume* GetPhysicalWorld()		{return worldPV;};
		const G4VPhysicalVolume* GetAbsorber()				{return absPV;};
		const G4VPhysicalVolume* GetGap()					{return gapPV;};

		void SetAbsorberMaterial(G4String);
		void SetAbsorberThickness(G4double);

		void SetGapMaterial(G4String);
		void SetGapThickness(G4double);

		void SetCaloRadius(G4double);
		void SetNumberOfLayers(G4int);

		void UpdateGeometry();
	private:
		//! Define needed materials
		void DefineMaterials();
		//! Initialzie geometry parametners
		void ComputeParameters();
		//! Creates the Calorimeter (detector)
		G4VPhysicalVolume* ConstructCalorimeter();
		//! Sets the visualtiation attributes
		void SetVisAttributes();

		// Geometry Names
		G4LogicalVolume* worldLV;			// Pointer to world logical volume
		G4VPhysicalVolume* worldPV;		// Pointer to world physical volume
		G4VSolid* worldS;						// Pointer to world's solid

		G4LogicalVolume* caloLV;			// Pointer to the calorimeter LV
		G4VPhysicalVolume* caloPV;			// Pointer to the calorimater PV
		G4VSolid* caloS;						// Pointer to the calorimeter soild

		G4LogicalVolume* layerLV;			// Pointer to a layer logical volume
		G4VPhysicalVolume* layerPV;		// Pointer to a layer physical volume
		G4VSolid* layerS;						// Pointer to a layer's solid

		G4LogicalVolume* absLV;				// Pointer to the absorber LV
		G4VPhysicalVolume* absPV;			// Pointer to the absorber PV
		G4VSolid*	absS;						// Pointer to the solid absorber

		G4LogicalVolume* gapLV;				// Pointer to the gap logical volume
		G4VPhysicalVolume* gapPV;			// Pointer to the gap physical volume
		G4VSolid* gapS;						// Pointer to the gap's solid 

		// Materials
		G4Material* defaultMaterial;    	// Vacumun
		G4Material* absMaterial;   		// Detector material
		G4Material* gapMaterial;        	// Gap material

		// Geometry parameters
		G4int nofLayers;              	// Number of Layers
		G4double absThickness;	      	// Thickness of Absorber
		G4double gapThickness;        	// Thickness of Gap 
		G4double oRadius;		    			// Outer Radius of Detector
		G4double iRadius;						// Inner radious of  Detector
		G4double startAngle;
		G4double spanAngle;

		G4double layerThickness;        // Thickness of a single layer
		G4double caloThickness;         // Thickness of entire calorimeter
		G4double worldSizeXY;
		G4double worldSizeZ;

		// data members
		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

