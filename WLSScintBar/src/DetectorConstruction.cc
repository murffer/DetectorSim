#include "G4ios.hh"
#include "globals.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4OpticalSurface.hh"
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
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

PhotonDetSD* DetectorConstruction::pmtSD = NULL;

DetectorConstruction::DetectorConstruction()
: physiWorld(NULL)
{
    
    detectorMessenger = new DetectorMessenger(this);
    materials = NULL;
    
    scintX = 100*um;
    scintY = 30*cm;
    scintZ = 200*cm;
    
    wlsThickness = 0.5*cm;
    cladThickness = 2*mm;
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
    detMaterial = FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    pmtMaterial = FindMaterial("BK7");
    wlsMaterial = FindMaterial("PMMA_WLS");
    cladMaterial = FindMaterial("G4_TEFLON");
    
    return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    // Want to check the overlaps (could turn this off once debuged)
    G4bool fCheckOverlap = true;
    
    //--------------------------------------------------
    // World
    //--------------------------------------------------
    
    solidWorld = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
    logicWorld = new G4LogicalVolume(solidWorld,FindMaterial("G4_AIR"),"World");
    physiWorld = new G4PVPlacement(0,G4ThreeVector(), logicWorld, "World", 0, false, fCheckOverlap);
    
    //--------------------------------------------------
    // Wrapping / Cladding
    //--------------------------------------------------
    G4double x = scintX + wlsThickness + cladThickness;
    G4double y = scintY + cladThickness;
    G4double z = scintZ + 2*pmtLength+cladThickness;
    solidClad = new G4Box("Cladding",x/2,y/2,z/2);
    logicClad = new G4LogicalVolume(solidClad,cladMaterial,"Cladding");
    physiClad = new G4PVPlacement(0, G4ThreeVector(), logicClad, "Cladding", logicWorld, 0, false,fCheckOverlap);
   opSurfClad = new G4OpticalSurface("Cladding");
   surfClad = new G4LogicalSkinSurface("Cladding",logicClad,opSurfClad);
    opSurfClad->SetType(dielectric_LUT);
    opSurfClad->SetModel(LUT);
    opSurfClad->SetFinish(polishedteflonair);
    
    //--------------------------------------------------
    // WLS Fibers
    //--------------------------------------------------
    solidWLS = new G4Box("WLS",(scintX+wlsThickness)/2,scintY/2,scintZ/2);
    logicWLS = new G4LogicalVolume(solidWLS,wlsMaterial,"WLS");
    physiWLS = new G4PVPlacement(0, G4ThreeVector(), logicWLS, "WLS", logicClad, 0, false,fCheckOverlap);
    
    //--------------------------------------------------
    // Scintillator
    //--------------------------------------------------
    solidScintillator = new G4Box("Scintillator",scintX/2,scintY/2,scintZ/2);
    logicScintillator = new G4LogicalVolume(solidScintillator,detMaterial,"Scintillator");
    physiScintillator = new G4PVPlacement(0, G4ThreeVector(), logicScintillator, "Scintillator", logicWLS, false,0, fCheckOverlap);
    
    //--------------------------------------------------
    // PhotonDet (Sensitive Detector)
    //--------------------------------------------------
    
    // Physical Construction
    G4double zOrig = (scintZ+pmtLength)/2;
    solidPhotonDetTop = new G4Box("PhotonDetTop",(scintX+wlsThickness)/2,scintY/2,pmtLength/2);
    logicPhotonDetTop = new G4LogicalVolume(solidPhotonDetTop,pmtMaterial, "PhotonDetTop");
    physiPhotonDetTop = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrig), logicPhotonDetTop, "PhotonDetTop", logicClad, false, 0, fCheckOverlap);
    
    solidPhotonDetBot= new G4Box("PhotonDetBot",(scintX+wlsThickness)/2,scintY/2,pmtLength/2);
    logicPhotonDetBot = new G4LogicalVolume(solidPhotonDetBot,pmtMaterial, "PhotonDetBot");
    physiPhotonDetBot = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,-zOrig), logicPhotonDetBot, "PhotonDetBot", logicClad, false, 0, fCheckOverlap);
    
    if (!pmtSD) {
        G4String pmtSDName = "/PhotonDet";
        pmtSD = new PhotonDetSD(pmtSDName);
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        SDman->AddNewDetector(pmtSD);
    }
    
    // Setting the detector to be sensitive
    logicPhotonDetTop->SetSensitiveDetector(pmtSD);
    logicPhotonDetBot->SetSensitiveDetector(pmtSD);
    PrintParameters();
    return physiWorld;
}
void DetectorConstruction::PrintParameters(){
    G4cout<<"Scintillator Slab:"
    <<"\n\t Thickness: "<<G4BestUnit(scintX,"Length")
    <<"\n\t Length: "<<G4BestUnit(scintZ,"Length")
    <<"\n\t Width: "<<G4BestUnit(scintY,"Length")
    <<"\n\t Material:"<<detMaterial->GetName()
    <<"\nPMT:"
    <<"\n\t Length: "<<G4BestUnit(pmtLength,"Length")
    <<"\n\t Material:"<<pmtMaterial->GetName()
    <<G4endl;
    
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
/**
 Updates the Geometry by deleting the old geoemtry and then starting over
 */
void DetectorConstruction::UpdateGeometry()
{
 //   if (!physiWorld) return;
    
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
    worldSizeX = scintX + 2*wlsThickness+cladThickness+1*cm;
    worldSizeY = scintY + 1*cm;
    worldSizeZ = scintZ + 2*pmtLength+2*cladThickness+5*cm;
    
}
void DetectorConstruction::SetScintThickness(G4double val){
    scintX = val;
}
/**
 * Sets the material of the cladding
 * @param material name
 */
void DetectorConstruction::SetCladMaterial(G4String name){
    cladMaterial = FindMaterial(name);
    if (opSurfClad){
    if(name == "G4_TEFLON"){
        opSurfClad->SetFinish(polishedteflonair);
    }else if(name == "G4_AIR"){
        opSurfClad->SetFinish(polishedair);
    }
    else if(name == "G4_MYLAR"){
        opSurfClad->SetFinish(polishedvm2000air);
    }else{
        G4cout<<"Material "<<name<<" is not in surface database."<<G4endl;
    }
    }
    else{
        G4cout<<"Did not set optical surface as surfrace is not yet created."<<G4endl;
    }
}
/**
 Sets the material of the WLS
 @param material name
 */
void DetectorConstruction::SetWLSMaterial(G4String name){
    wlsMaterial = FindMaterial(name);
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
