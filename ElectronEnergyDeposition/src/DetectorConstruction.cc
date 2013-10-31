#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

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

/**
 * Constructs the semi-infinite box 
 *
 * Defaults are a 10 m box of G4_Water without a magnetic field
 */
DetectorConstruction::DetectorConstruction() :fPBox(0), fLBox(0), fMaterial(0)
{
  fBoxSize = 10*m;
  // Creating Detector Materials
  materials = Materials::GetInstance();
  SetMaterial("G4_WATER");  
  fDetectorMessenger = new DetectorMessenger(this);
}

/**
 * Deconstructor
 */
DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

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
  G4Box* sBox = new G4Box("World",fBoxSize/2,fBoxSize/2,fBoxSize/2);
  fLBox = new G4LogicalVolume(sBox,fMaterial,"World");
  fPBox = new G4PVPlacement(0,G4ThreeVector(),fLBox,fMaterial->GetName(),0,false,0);

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
 * Sets the detector size
 *
 * @param value - lenght of an edge of a box
 */
void DetectorConstruction::SetSize(G4double value)
{
  fBoxSize = value;
}
/**
 * Updates the geometry by constructing a new world
 */
#include "G4RunManager.hh"
void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}
