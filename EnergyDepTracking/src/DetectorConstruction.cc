#include <vector>

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

#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include "Analysis.hh"
/**
 * Constructs the semi-infinite box 
 *
 * Defaults are a 10 m box of G4_POLYSTYRENE
 *
 * The number of voxels or discretizition for the energy depostion is set with 
 * the FNumChambers parameter.  This number should be an odd number in order to
 * have the 0,0 voxel at the origin of the particle gun.
 */
DetectorConstruction::DetectorConstruction() :fPBox(0), fLBox(0), fMaterial(0)
{
  fBoxSize = 1*cm;
  fNumChambers=101;
  fMaxStep=1*um;
  // Creating Detector Materials
  materials = Materials::GetInstance();
  SetMaterial("G4_POLYSTYRENE");  
}

/**
 * Deconstructor
 *
 * Cleaning up the logical volume vector
 */
DetectorConstruction::~DetectorConstruction()
{
  std::vector<G4LogicalVolume*>::iterator it;
  for (it = fLogicChamber.begin(); it != fLogicChamber.end(); it++){
    delete *it;
  }
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
  fLBox = new G4LogicalVolume(sBox,fMaterial,"Wordl");
  fPBox = new G4PVPlacement(0,G4ThreeVector(),fLBox,fMaterial->GetName(),0,false,false);

  // Creating the Sensitive Detector
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SD = new SensitiveDetector("SD/SD","HitCollection");
  SDman->AddNewDetector(SD);

  // Creating tracking chambers
  G4double voxelSize = fBoxSize/fNumChambers;
  G4int copyNum = 0;
  for(G4double x = -fBoxSize/2; x < fBoxSize/2; x += voxelSize){
    for(G4double y = -fBoxSize/2; y < fBoxSize/2; y += voxelSize){
      // Creating the solid and logical volumes
      G4Box* solid = new G4Box("chamber",voxelSize/2,voxelSize/2,fBoxSize/2);
      G4LogicalVolume* log = new G4LogicalVolume(solid,fMaterial,"Chamber");

      // initilizaiton and adding to geoemtry
      log->SetSensitiveDetector(SD);
      log->SetUserLimits(new G4UserLimits(fMaxStep));
      new G4PVPlacement(0,G4ThreeVector(x+voxelSize/2,y+voxelSize/2,0),log,"Chamber",fLBox,false,copyNum,false);

      // Adding to storage and incrementing
      fLogicChamber.push_back(log);
      copyNum++;
    }
  }
  // Setting up the analysis
  Analysis::GetInstance()->SetNumberVoxels(copyNum);
  Analysis::GetInstance()->SetupVoxelPositions();
  copyNum = 0;
  for(G4double x = -fBoxSize/2; x < fBoxSize/2; x += voxelSize){
    for(G4double y = -fBoxSize/2; y < fBoxSize/2; y += voxelSize){
      Analysis::GetInstance()->SetVoxelPosition(copyNum,x+voxelSize/2,y+voxelSize/2);
      copyNum++;
    }
  }
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
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}
