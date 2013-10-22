#include "G4ios.hh"
#include "globals.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4GeometryManager.hh"
#include "G4SDManager.hh"

#include "G4SolidStore.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "Materials.hh"
#include "PhotonDetSD.hh"

#include "G4UserLimits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

PhotonDetSD* DetectorConstruction::pmtSD = NULL;

DetectorConstruction::DetectorConstruction()
: physiWorld(NULL)
{
    
    detectorMessenger = new DetectorMessenger(this);
    materials = NULL;
    
    pmtPolish = 0.;
    pmtReflectivity = 0.;
    
    scintX = 30*cm;
    scintY = 10*cm;
    scintZ = 200*cm;
    
    pmtLength = 0.5*cm;

    UpdateGeometryParameters();
}

DetectorConstruction::~DetectorConstruction()
{
    if (detectorMessenger) delete detectorMessenger;
    if (materials)         delete materials;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    materials = Materials::GetInstance();
    
    return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
    G4bool fCheckOverlap = true;
    //--------------------------------------------------
    // World
    //--------------------------------------------------
    
    G4VSolid* solidWorld = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
    logicWorld = new G4LogicalVolume(solidWorld,FindMaterial("G4_AIR"),"World");
    physiWorld = new G4PVPlacement(0,G4ThreeVector(), logicWorld, "World", 0, false, fCheckOverlap);
    
    //--------------------------------------------------
    // Scintillator
    //--------------------------------------------------
    G4VSolid* solidScintillator = new G4Box("Scintillator",scintX/2,scintY/2,scintZ/2);
    G4LogicalVolume* logicScintillator = new G4LogicalVolume(solidScintillator,FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"),"Scintillator");
    new G4PVPlacement(0, G4ThreeVector(), logicScintillator, "Scintillator", logicWorld, false,0, fCheckOverlap);
    
    
    //--------------------------------------------------
    // PhotonDet (Sensitive Detector)
    //--------------------------------------------------
    
    // Physical Construction
    G4double zOrig = (scintZ+pmtLength)/2;
    G4VSolid* solidPhotonDet = new G4Box("PhotonDet",scintX/2,scintY/2,pmtLength/2);
    G4LogicalVolume*   logicPhotonDet = new G4LogicalVolume(solidPhotonDet, FindMaterial("BK7"), "PhotonDet");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrig), logicPhotonDet, "PhotonDet", logicWorld, false, 0, fCheckOverlap);
    
    // PhotonDet Surface Properties
    /*
    G4OpticalSurface* PhotonDetSurface = new G4OpticalSurface("PhotonDetSurface", glisur, ground,dielectric_metal,pmtPolish);
    G4MaterialPropertiesTable* PhotonDetSurfaceProperty =new G4MaterialPropertiesTable();
    
    G4double p_pmt[2] = {2.00*eV, 3.47*eV};
    G4double refl_pmt[2] = {pmtReflectivity,pmtReflectivity};
    G4double effi_pmt[2] = {1, 1};
    
    PhotonDetSurfaceProperty -> AddProperty("REFLECTIVITY",p_pmt,refl_pmt,2);
    PhotonDetSurfaceProperty -> AddProperty("EFFICIENCY",p_pmt,effi_pmt,2);
    
    PhotonDetSurface -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
    new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet,PhotonDetSurface);
    */
    if (!pmtSD) {
        G4cout<<"Trying to add the Sensitived Detector"<<G4endl;
        G4String pmtSDName = "/PhotonDet";
        pmtSD = new PhotonDetSD(pmtSDName);
        G4cout<<"Adding the Sensitived Detector"<<G4endl;
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        SDman->AddNewDetector(pmtSD);
    }
    
    // Setting the detector to be sensitive
    logicPhotonDet->SetSensitiveDetector(pmtSD);
    
    return physiWorld;
}

/**
 * Sets the material of the scintillating slab
*/
void DetectorConstruction::SetScintMaterial(G4String matName){
    detMaterial = FindMaterial(matName);
}
/**
 * Sets the material of the PMT
 */
void DetectorConstruction::SetPMTMaterial(G4String matName){
    pmtMaterial = FindMaterial(matName);
}

void DetectorConstruction::UpdateGeometry()
{
    if (!physiWorld) return;
    
    // clean-up previous geometry
    G4GeometryManager::GetInstance()->OpenGeometry();
    
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();
    
    //define new one
    UpdateGeometryParameters();
    
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
    
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    
    G4RegionStore::GetInstance()->UpdateMaterialList(physiWorld);
}

void DetectorConstruction::UpdateGeometryParameters()
{
    worldSizeX = scintX + 1*cm;
    worldSizeY = scintY + 1*cm;
    worldSizeZ = scintZ + 2*pmtLength+5*cm;
    
}
void DetectorConstruction::SetScintThickness(G4double val){
    scintX = val;
}

/**
 Sets the polish of the photon detector
 @param polish (0 < polish <= 1) a polish of 1.0 is a perfect mirror surface
 */
void DetectorConstruction::SetPhotonDetPolish(G4double polish)
{
    pmtPolish = polish;
}

/**
 Sets the reflectivity of the photon dector
 @param reflectivity (0 < refelctivity <= 1) - a reflectivtiy of 1 is a perfect mirror
 */
void DetectorConstruction::SetPhotonDetReflectivity(G4double reflectivity)
{
    pmtReflectivity = reflectivity;
}

/**
 * Gets material
 * @param name of the material
 * @return a pointer to the material, or NULL if not found
 */
G4Material* DetectorConstruction::FindMaterial(G4String name) {
  
  G4Material* pttoMaterial = materials->GetMaterial(name);
  return pttoMaterial;
}
