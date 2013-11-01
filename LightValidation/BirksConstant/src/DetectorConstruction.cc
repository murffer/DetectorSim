#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
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
 * Creating the detector's world volume. The default is a 
 * box of GS20 that is 5 cm on a side.
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){
    
    // Creating Detector Materials
    materials = Materials::GetInstance();
    
    // Defaults
    detMaterial = FindMaterial("GS20");
    detectorThickness = 5*cm;
    
    // Physical World
    G4VPhysicalVolume* world = ConstructVolumes();
    
    return world;
}

/**
 * PrintCaloParameters
 *
 * Prints the parameters of the geometry
 */
void DetectorConstruction::PrintParameters(){
    
    // print parameters
    G4cout<<"\n------------ Detector Parameters ---------------------"
    <<"\n\t--> Detector:"
    <<"\n\t\tMaterial:  "<<detMaterial->GetName()
    <<"\n\t\tThickness: "<<G4BestUnit(detectorThickness,"Length")
    <<G4endl;
}
/**
 * FindMaterial
 *
 * Finds, and if necessary, builds a material
 */
G4Material* DetectorConstruction::FindMaterial(G4String name){
    return materials->GetMaterial(name);
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
    
    G4double worldSize = 1.1*detectorThickness;

    
    /* World */
    worldS = new G4Box("World",worldSize/2,worldSize/2,worldSize/2);
    worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);

    /* Detector */
    detS = new G4Box("Detector",detectorThickness/2,detectorThickness/2,detectorThickness/2);
    detLV = new G4LogicalVolume(detS,detMaterial,"Detector");
    detPV = new G4PVPlacement(0,G4ThreeVector(),detLV,"Detector",worldLV,false,0,fCheckOverlaps);
    
    
    // Printing basic information about the geometry
    PrintParameters();
    
    
    // Return the worlds physical volume
    return worldPV;
}


/**
 * SetAbsorberThickness
 *
 * Sets the detector thickness
 */
void DetectorConstruction::SetDetectorThickness(G4double val){
    detectorThickness = val;
}

/**
 * SetDetectorMaterial
 * 
 * Sets the detector material
 */
void DetectorConstruction::SetDetectorMaterial(G4String name){
    detMaterial = FindMaterial(name);
}

/**
 * Sets the Birks Constant of the scinilllator
 * @param birks constant, in mm/MeV
 */
void DetectorConstruction::SetBirksConstant(G4double val){
    detMaterial->GetIonisation()->SetBirksConstant(val);
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
    G4RegionStore::GetInstance()->UpdateMaterialList(worldPV);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();

}
