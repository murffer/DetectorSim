#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
#include "G4UnitsTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


#include "CaloSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"

#include <iostream>
#include <string>
#include <sstream>
#include "Analysis.hh"

/**
 * DetectorConstruction
 *
 * Setting the default detector construciton.
 */
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
  fCheckOverlaps(true){

    worldSizeZ  = 25*cm;      // Fixed World Size; 

    // Geometry parameters
    absThickness = 2*cm;	        // Thickness of Absorber
    gapThickness = 1*cm;            // Thickness of Gap 
    oRadius  = 2.54*cm;		        // Outer Radius of Detector
    iRadius = 0.*cm;				// Inner radious of  Detector
    startAngle = 0.*deg;
    spanAngle = 360.*deg;

    // Compute parameters
    ComputeParameters();

    // Define materials 
    materials = Materials::GetInstance();
    SetAbsorberMaterial("PSLiF");
    SetGapMaterial("G4_PLEXIGLASS");
    defaultMaterial = G4Material::GetMaterial("Galactic");

    // Create commands for interactive defiantions of the calorimeter
    detectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction(){
  delete detectorMessenger;
}

/**
 * Construct
 *
 * Creating the detector's world volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){

  // Return Physical World
  G4VPhysicalVolume* calo = ConstructCalorimeter();

  // Set Visulation Attributes
  SetVisAttributes();

  // Assign Sensitve Detectors
  SetSensitiveDetectors();
  return calo;
}


/**
 * ComputeParameters
 *
 */
void DetectorConstruction::ComputeParameters(){

  caloThickness = absThickness+2*gapThickness;
  worldSizeXY = 1.2 * oRadius;
}

/**
 * PrintCaloParameters
 *
 * Prints the parameters of the geometry
 */
void DetectorConstruction::PrintCaloParameters(){

  // print parameters
  G4cout << "\n------------ Calorimeter Parameters ---------------------"
    <<"\n--> The carlorimeter is a single layer of: \n\t[ "
    << G4BestUnit(absThickness,"Length")<< " of " << absMaterial->GetName() 
    << " + "
    << G4BestUnit(gapThickness,"Length") << " of " << gapMaterial->GetName() << " ]"
    << "\n--> The calormeter is " <<G4BestUnit(caloThickness,"Length") << " thick"
    << " with a radius of "<<G4BestUnit(oRadius,"Length")<<""
    << "\n------------------------------------------------------------\n"
    <<" The world is "<<worldSizeXY/cm<<" cm by "<<worldSizeXY/cm 
    <<" cm by "<<worldSizeZ/cm<<" cm."
    <<G4endl;
}

/**
 * ConstructCalorimeter()
 *
 * Calorimeter is constructed as a solid cylinder of the gap (non-scintillating)
 * material with layers of the absorber (scintillating) material.
 */
G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter(){

  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Calor parameters definition
  ComputeParameters();


  // World
  worldS = new G4Box("World",worldSizeXY, worldSizeXY, worldSizeZ); 
  worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
  worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
      0,false,0,fCheckOverlaps);

  //
  // Setting up the Calorimeter
  //
  // The beam is shotting along the z, comming from +z

  // Absorber
  absS = new G4Tubs("Abs",iRadius,oRadius,absThickness/2,0,360*deg);
  absLV = new G4LogicalVolume(absS,absMaterial,"Absorber",0);
  absPV = new G4PVPlacement(0,G4ThreeVector(0,0,(gapThickness+absThickness)/2),
      absLV,"Absorber",worldLV,false,0,fCheckOverlaps);

  // Spacer / gap
  gapS = new G4Tubs("Gap",iRadius,oRadius,gapThickness/2,0,360*deg);
  gapLV = new G4LogicalVolume(gapS,gapMaterial,"Gap",0);
  gapPV = new G4PVPlacement(0,G4ThreeVector(),
      gapLV,"Gap",worldLV,false,0,fCheckOverlaps);

  PrintCaloParameters();

  // Return the worlds physical volume
  return worldPV;
}
/**
 * SetSensitiveDetectors
 *
 * Setting the Sensitive Detectors of the Detector
 */
void DetectorConstruction::SetSensitiveDetectors(){
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  caloSD = new CaloSensitiveDetector("SD/CaloSD","CaloHitCollection");
  SDman->AddNewDetector(caloSD);
  absLV->SetSensitiveDetector(caloSD);

  // Setting the Maximum Step Size
  G4double maxStep = 0.01*absThickness;
  absLV->SetUserLimits(new G4UserLimits(maxStep));
}
/**
 * SetVisAttributes()
 *
 * Sets the visualtion attributes
 */
#include "G4Colour.hh"
void DetectorConstruction::SetVisAttributes(){

  // Setting the Visualization attributes for the Abs
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Cyan());
    //atb->SetForceWireframe(true);
    atb->SetForceSolid(true);
    absLV->SetVisAttributes(atb);}

    // Setting the Visualization attributes for the Calorimeter 
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
      //atb->SetForceWireframe(true);
      atb->SetForceSolid(true);
      gapLV->SetVisAttributes(atb);}

      // Setting the Layers to be white and invisiable
      {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
        atb->SetForceWireframe(true);
        atb->SetVisibility(false);
        worldLV->SetVisAttributes(atb);}

        // Setting the World to be white and invisiable
        {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
          atb->SetForceWireframe(true);
          atb->SetVisibility(false);
          worldPV->GetLogicalVolume()->SetVisAttributes(atb);}

}

/**
 * SetAbsorberMaterial
 *
 * Set's the absorber material, which contains the neutron absorber
 */
void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice){
  // search the material by its name
  G4Material* pttoMaterial = materials->GetMaterial(materialChoice);
  if (pttoMaterial) absMaterial = pttoMaterial;
}
/**
 * SetGapMaterial
 *
 * Set's the gap material.
 */
void DetectorConstruction::SetGapMaterial(G4String materialChoice){
  // search the material by its name
  G4Material* pttoMaterial = materials->GetMaterial(materialChoice);
  if (pttoMaterial) gapMaterial = pttoMaterial;
}

/**
 * SetAbsorberThickness
 *
 * Sets the absorber thickness
 */
void DetectorConstruction::SetAbsorberThickness(G4double val){
  absThickness = val;
}

/**
 * SetGapThickness
 *
 * Sets the thickness of the gap
 */
void DetectorConstruction::SetGapThickness(G4double val){
  gapThickness = val;
}

/**
 * SetCaloRadius
 *
 * Sets the calorimter radius
 */
void DetectorConstruction::SetCaloRadius(G4double val){
  oRadius = val;
}

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
  SetVisAttributes();
  absLV->SetSensitiveDetector(caloSD);

  // Setting the Maximum Step Size
  G4double maxStep = 0.01*absThickness;
  absLV->SetUserLimits(new G4UserLimits(maxStep));

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
