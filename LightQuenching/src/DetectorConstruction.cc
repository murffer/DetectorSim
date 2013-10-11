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
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

#include "G4SDManager.hh"
#include "AbsorberSD.hh"
#include "PMTSD.hh"

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
 * Creating the detector's world volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){

  // Creating Detector Materials
  materials = Materials::GetInstance();

  // Return Physical World
  G4VPhysicalVolume* world = ConstructVolumes();

  // Sets up the Optical Surfaces
  SetOpticalSurfaces();

  // Set Visulation Attributes
  SetVisAttributes();

  // Assign Sensitve Detectors
  SetSensitiveDetectors();
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
    <<"\n--> The detector material is a disc of: "<<absMaterial->GetName()
    <<"\n\t thickness: "<<G4BestUnit(gs20Thickness,"Length")
    <<"\n\t radius: "<<G4BestUnit(gs20Radius,"Length")
    <<"\n--> Mounting Material: "<<mountMaterial->GetName()
    <<"\n\t thickness: "<<G4BestUnit(refThickness,"Length")
    <<"\n--> PMT Material: "<<pmtMaterial->GetName()
    <<"\n\t thickness: "<<G4BestUnit(pmtThickness,"Length")
    <<"\n\t radius: "<<G4BestUnit(pmtRadius,"Length")
    <<"\n--> Reflector Material: "<<refMaterial->GetName()
    <<"\n\t thickness: "<<G4BestUnit(refThickness,"Length")
    <<G4endl;
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

  worldSizeZ  = 5*cm;      // Fixed World Size; 

  // Geometry parameters
  gs20Thickness = 2*mm;	          /* Thickness of GS20 Scintilator  */
  gs20Radius  = 1.27*cm;		      /* Radius of GS20 Sctintillator   */
  pmtRadius = 2.54*cm;
  pmtThickness = 5*mm;
  mountThickness = 100*um;
  refThickness = 3.33*mm;
  capThickness = 2*mm;                       /* Thickness of the cap     */
  absMaterial = FindMaterial("GS20");
  pmtMaterial = FindMaterial("BK7");
  refMaterial = FindMaterial("G4_TEFLON");
  mountMaterial = FindMaterial("Silicone");
  G4double zTran;

  // The constructors take half thickness, so divide by two for them
  // World
  worldS = new G4Box("World",2.5*pmtRadius,2.5*pmtRadius,10*cm); 
  worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
  worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);

  // GS20 Detector
  gs20S = new G4Tubs("Abs",0,gs20Radius,gs20Thickness/2,0,360*deg);
  gs20LV = new G4LogicalVolume(gs20S,absMaterial,"Absorber",0);
  gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,0),gs20LV,"Absorber - GS20",worldLV,false,0,fCheckOverlaps);

  // Abosrber and PMT Mounting (Optical Grease)
  mountS = new G4Tubs("opticalGrease",0,gs20Radius,mountThickness/2,0,360*deg);
  mountLV = new G4LogicalVolume(mountS,mountMaterial,"PMT Glass",0);
  zTran = -(gs20Thickness+mountThickness)/2;
  mountPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),mountLV,"Grease",worldLV,false,0,fCheckOverlaps);

  // PMT Glass
  pmtS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness/2,0,360*deg);
  pmtLV = new G4LogicalVolume(pmtS,pmtMaterial,"PMT Glass",0);
  zTran = -(gs20Thickness/2+pmtThickness/2+mountThickness);
  pmtPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),pmtLV,"PMTGlass",worldLV,false,0,fCheckOverlaps);

  // Light Reflector (Teflon)
  G4Tubs *refSide = new G4Tubs("refSide",gs20Radius,gs20Radius+refThickness,(refThickness+gs20Thickness+mountThickness)/2,0,360*deg);
  G4Tubs *refTop = new G4Tubs("refTop",0,gs20Radius,refThickness/2,0,360*deg);
  refS = new G4UnionSolid("Reflector",refSide,refTop,0,G4ThreeVector(0,0,(gs20Thickness+mountThickness)/2));
  refLV = new G4LogicalVolume(refS,refMaterial,"Reflector",0);
  zTran = (refThickness-mountThickness)/2;
  refPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),refLV,"Reflector",worldLV,false,0,fCheckOverlaps);

  // PMT Cap
  G4double capLength = gs20Thickness+refThickness+pmtThickness+mountThickness+capThickness;
  G4Tubs *capSide = new G4Tubs("CapSide",pmtRadius,pmtRadius+capThickness,capLength/2,0,2*pi);
  G4Tubs *capTop = new G4Tubs("CapTop",0,pmtRadius,capThickness/2,0,2*pi);
  pmtCapS = new G4UnionSolid("PMTCap",capSide,capTop,0,G4ThreeVector(0,0,(capLength-capThickness)/2));
  pmtCapLV = new G4LogicalVolume(pmtCapS,FindMaterial("G4_POLYVINYL_CHLORIDE"),"PMT Cap",0);
  zTran = (refThickness-pmtThickness-mountThickness+capThickness)/2;
  pmtCapPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),pmtCapLV,"PMTCap",worldLV,false,0,fCheckOverlaps);

  // PMT Air
  pmtAirS = new G4Tubs("PMTAir",gs20Radius+refThickness,pmtRadius,(gs20Thickness+mountThickness+refThickness)/2,0,2*pi);
  pmtAirLV = new G4LogicalVolume(pmtAirS,FindMaterial("G4_AIR"),"PMT Air Gap",0);
  zTran = (refThickness-mountThickness)/2;
  pmtAirPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),pmtAirLV,"PMTAir",worldLV,false,0,fCheckOverlaps);

  // Printing basic information about the geometry
  PrintParameters();

  // Return the worlds physical volume
  return worldPV;
}
/**
 * SetsOpticalSurfaces
 *
 * Sets the optical surfaces for the detector volumes
 */
void DetectorConstruction::SetOpticalSurfaces(){
  // Reflector (Teflon) Optical Surface
  G4OpticalSurface *refOpSurface = new G4OpticalSurface("Reflector Surface");
  refOpSurface->SetType(dielectric_LUT);
  refOpSurface->SetModel(LUT);
  refOpSurface->SetFinish(polishedteflonair);
  G4LogicalSkinSurface *refLogSurface = new G4LogicalSkinSurface("Reflector Logical Skin Surface",refLV,refOpSurface);

  // Detector Mounting and Silicon Grease Optical Surface (TODO)
  // This may not be necessary as we can almost assume perfect coupling between the GS20 and the PMT due to
  // the optical grease. 
}
/**
 * SetSensitiveDetectors
 *
 * Setting the Sensitive Detectors of the Detector.
 * If the sensitive detectors exits, then only the senstive detector is 
 * registered to the logical volume.
 */
void DetectorConstruction::SetSensitiveDetectors(){
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if (!pmtSD){
    pmtSD = new PMTSD("PMT/PMTSD","PMTHitCollection");
    SDman->AddNewDetector(pmtSD);
  }
  if (!absSD){
    absSD = new AbsorberSD("Absorber/AbsSD","AbsHitCollection");
    SDman->AddNewDetector(absSD);
  }
  gs20LV->SetSensitiveDetector(absSD);
  pmtLV->SetSensitiveDetector(pmtSD);
}
/**
 * SetVisAttributes()
 *
 * Sets the visualtion attributes
 */
#include "G4Colour.hh"
void DetectorConstruction::SetVisAttributes(){

  // Setting the Visualization attributes for the Absorber (scintillator)
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Blue());
    atb->SetForceSolid(true);
    gs20LV->SetVisAttributes(atb);}

  // Setting the PMT to be green
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::Green());
    atb->SetForceSolid(true);
    pmtLV->SetVisAttributes(atb);}

  // Setting the mounting (optical grease) to be grey
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::Grey());
    atb->SetForceSolid(true);
    mountLV->SetVisAttributes(atb);}

  // Setting the Teflon to be cyan
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::Cyan());
    //atb->SetForceSolid(true);
    //atb->SetForceWireframe(true);
    refLV->SetVisAttributes(atb);}

  // Setting the PMT to be yellow 
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::Yellow());
    //atb->SetForceSolid(true);
    //atb->SetForceWireframe(true);
    pmtCapLV->SetVisAttributes(atb);}

  // Setting the PMT Air to be red 
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::Red());
    //atb->SetForceSolid(true);
    //atb->SetForceWireframe(true);
    pmtAirLV->SetVisAttributes(atb);}

  // Setting the World to be white and invisiable
  {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
    //atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldPV->GetLogicalVolume()->SetVisAttributes(atb);}
}

/**
 * SetMountingThickness
 *
 * Sets the thickness of the mounting layer (optical grease)
 */
void DetectorConstruction::SetMountingThickness(G4double val){
  mountThickness = val;
}

/**
 * SetReflectorThickness
 *
 * Sets the thickness of the light reflector (teflon tape)
 */
void DetectorConstruction::SetReflectorThickness(G4double val){
  refThickness = val;
}

/**
 * SetGS20Thickness
 *
 * Sets the detector thickness
 */
void DetectorConstruction::SetGS20Thickness(G4double val){
  gs20Thickness = val;
}

/**
 * SetGS20Radius
 *
 * Sets the calorimter radius
 */
void DetectorConstruction::SetGS20Radius(G4double val){
  if (val > pmtRadius){
    G4cout<<"Warning: tring to replace GS20 Radius with a radius that is bigger than the PMT"<<G4endl;
  } 
  else
    gs20Radius = val;
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
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  // Creating the new geomtry  
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
  SetOpticalSurfaces();
  SetSensitiveDetectors(); 
  SetVisAttributes();

  // Updating the engine
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RegionStore::GetInstance()->UpdateMaterialList(worldPV);
}
