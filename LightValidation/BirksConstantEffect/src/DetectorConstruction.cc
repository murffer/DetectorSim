#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4UserLimits.hh"

/**
 * DetectorConstruction
 *
 * Setting the default detector construciton.
 */
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
  fCheckOverlaps(true){

    // Create commands for interactive defiantions of the calorimeter
    detectorMessenger = new DetectorMessenger(this);
  }

/**
 * Deconstructor
 */
DetectorConstruction::~DetectorConstruction(){
  // Deleting the messenger and materials if they exist
  if (detectorMessenger) delete detectorMessenger;
  if (materials)         delete materials;
}

/**
 * Construct
 *
 * Creating the detector's world volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){

  // Creating Detector Materials
  materials = Materials::GetInstance();

  // Return Physical World
  G4VPhysicalVolume* world = ConstructVolumes();
  return world;
}

/**
 * FindMaterial
 *
 * Finds, and if necessary, builds a material
 */
G4Material* DetectorConstruction::FindMaterial(G4String name){
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
}
/**
 * Construct()
 *
 * Constructs the detector volume and PMT
 */
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes(){
  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // World
  worldS = new G4Tubs("World",0,3*cm,2*cm,0,360*deg); 
  worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
  worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);

  // GS20 Detector
  gs20S = new G4Tubs("Abs",0,1.27*cm,1*mm,0,360*deg);
  gs20LV = new G4LogicalVolume(gs20S,absMaterial,"Absorber",0);
  gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,0),gs20LV,"Absorber - GS20",worldLV,false,0,fCheckOverlaps);


  // Return the worlds physical volume
  return worldPV;
}

/**
 * Sets the Birks Parameter of the scintilliator
 *
 * @param value - the Birks parameter
 */
void DetectorConstruction::SetBirksParameter(G4double val){
		// Expects units of mm/MeV
    absMaterial->GetIonisation()->SetBirksConstant(val*mm/MeV);
}
/**
 * UpdateGeometry
 *
 * Creates a new geometry, and reassings the sensitive detectors
 */
void DetectorConstruction::UpdateGeometry(){

  if(!worldPV) return;

  // Cleaning up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();

  // Creating the new geomtry  
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());

  // Updating the engine
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RegionStore::GetInstance()->UpdateMaterialList(worldPV);
}
