#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "DetectorSD.hh"
#include "G4SDManager.hh"
/**
 * Constructs the semi-infinite box 
 *
 * Defaults are a 10 cm box of Plastic Scintillator
 *
 * The number of voxels or discretizition for the energy depostion is set with 
 * the FNumChambers parameter.  This number should be an odd number in order to
 * have the 0,0 voxel at the origin of the particle gun.
 */
DetectorConstruction::DetectorConstruction() :fPBox(0), fLBox(0), fMaterial(0)
{
  fBoxSize = 10*cm;
  // Creating Detector Materials
  materials = Materials::GetInstance();
  SetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
}

/**
 * Deconstructor
 *
 */
DetectorConstruction::~DetectorConstruction()
{
}

/**
 * Constructs the detector volume
 *
 * @return constructed detector volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}
/**
 * Construct the detector volumes (a box). If any old geometries are there they
 * are cleaned up before the new root volume is returned.
 *
 * @return - pointer to physical volume
 */
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Creating the new geoemtry
  G4Box* sBox = new G4Box("Container",fBoxSize/2,fBoxSize/2,fBoxSize/2);
  fLBox = new G4LogicalVolume(sBox,fMaterial,"World");
  fPBox = new G4PVPlacement(0,G4ThreeVector(),fLBox,fMaterial->GetName(),0,false,false);

    // Setting the Senstive Detector
     detSD = new WLSPhotonDetSD("LYQuench/PhotonDe");
      fLBox->SetSensitiveDetector(detSD);

     G4SDManager* SDman = G4SDManager::GetSDMpointer();
     SDman->AddNewDetector(detSD);
  
  PrintParameters();
  
  //always return the root volume
  return fPBox;
}
/**
 * Prints the detector parameters
 */
void DetectorConstruction::PrintParameters()
{
  G4cout << "\n The Box is " << G4BestUnit(fBoxSize,"Length")
    << " of " << fMaterial->GetName() << G4endl;
}
/**
 * Sets the detector material
 *
 * @param materialChoice - name of the material
 */
void DetectorConstruction::SetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = materials->GetMaterial(materialChoice);

  if (pttoMaterial) { fMaterial = pttoMaterial;
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
      << materialChoice << " not found" << G4endl;
  }              
}
/**
 * Updates the geometry by constructing a new world
 */
#include "G4RunManager.hh"
void DetectorConstruction::UpdateGeometry()
{
  if (!fPBox) return;
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  G4RegionStore::GetInstance()->UpdateMaterialList(fPBox);
}
