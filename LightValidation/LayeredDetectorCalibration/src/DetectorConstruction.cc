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
    <<"\n--> RPM8:"
    <<"\n\tThickness (x): "<<G4BestUnit(rpm8Thickness,"Length")
    <<"\n\tWidth (y):     "<<G4BestUnit(rpm8Width,"Length")
    <<"\n\tHeight (z):    "<<G4BestUnit(rpm8Height,"Length")
    <<"\n--> Detector Slice (O M A M O):"
    <<"\n\tThickness: "<<G4BestUnit(sliceThickness,"Length")
    <<"\n\t--> Abosrber (A):"
    <<"\n\t\tMaterial (1x):  "<<absMaterial->GetName()
    <<"\n\t\tThickness: "<<G4BestUnit(detectorThickness,"Length")
    <<"\n\t--> Detector Mounting (M):"
    <<"\n\t\tMaterial:  "<<detMountMaterial->GetName()
    <<"\n\t\tThickness (2x): "<<G4BestUnit(detMountThickness,"Length")
    <<"\n\t--> Optical Mounting (O):"
    <<"\n\t\tMaterial:  "<<mountMaterial->GetName()
    <<"\n\t\tThickness (2x): "<<G4BestUnit(mountThickness,"Length")
    <<"\n--> Light Guide:"
    <<"\n\tMaterial: "<<lightGuideMaterial->GetName()
    <<"\n\tHeight:   "<<G4BestUnit(lightGuideHeight,"Length")
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

  // Geometry parameters
  pmtRadius = 2.54*cm;
  pmtThickness = 5*mm;
  mountThickness = 100*um;
  detMountThickness = 1*mm;
  refThickness = 3.33*mm;
  rpm8Width = 30.5*cm;       /* Width of the RPM8 SAIC Detector Module   */
  rpm8Height = 221.0*cm;     /* Height of the RPM8 SAIC Detector Module  */
  rpm8Thickness = 12.7*cm;   /* Thickness of the RPM8 SAIC Detector      */
  srcRadius = 2.51*um;       /* Cf-252 Source Radius                     */
  pbSrcThickness = 0.5*cm;   /* Pb thickness around Cf252 source         */
  modSrcThickness = 2.5*cm;  /* HDPE moderator around Cf252 source       */
  detectorThickness = 100*um;
  sliceThickness = detectorThickness+2*(detMountThickness+mountThickness);
  lightGuideHeight = 5*cm;
  absMaterial = FindMaterial("EJ426");
  pmtMaterial = FindMaterial("BK7");
  refMaterial = FindMaterial("G4_TEFLON");
  mountMaterial = FindMaterial("Silicone");
  detMountMaterial = FindMaterial("G4_PLEXIGLASS");
  lightGuideMaterial = FindMaterial("G4_PLEXIGLASS");

  /* World */
  // The RPM SAIC footprint is 12.7 cm x 30.5 cm x 221.0 cm, and the source
  // needs todd be two meters (x) from the detector midpoint
  worldS = new G4Box("World",2.5*m/2,50*cm/2,2.5*m/2); 
  worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
  worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);
  {
    G4VisAttributes* atb= new G4VisAttributes(G4Colour::Yellow());
    atb->SetVisibility(false);
    worldLV->SetVisAttributes(atb);
  }
  /* Creating and Positing the Source */
  G4ThreeVector srcTran(-1*m,0,0);
  sourceLV = ConstructSource();
  sourcePV = new G4PVPlacement(0,srcTran,sourceLV,"HDPE encased source",worldLV,false,0,fCheckOverlaps);

  /* Creating and Positing the RMP8 */
  G4ThreeVector rpm8Tran(1*m,0,0);
  std::list<G4double> slices;
  slices.push_back(1.96*cm);
  slices.push_back(2.49*cm);
  slices.push_back(4.9*cm);
  slices.push_back(6.86*cm);
  rpm8LV = ConstructRPM8(slices);
  rpm8PV = new G4PVPlacement(0,rpm8Tran,rpm8LV,"RPM8 Detector",worldLV,false,0,fCheckOverlaps);

  /**
   * Setting up the Light Guides and PMT's 
   */
  G4double lgZTran = rpm8Height/2+lightGuideHeight/2; 
  G4VSolid* lightGuideTopS = new G4Trd("Light Guide",rpm8Thickness/2,pmtRadius,rpm8Width/2,pmtRadius,lightGuideHeight/2);
  G4LogicalVolume* lightGuideTopLV = new G4LogicalVolume(lightGuideTopS,lightGuideMaterial,"Light Guide");
  new G4PVPlacement(0,rpm8Tran+G4ThreeVector(0,0,lgZTran),lightGuideTopLV,"Top Light Guide",worldLV,false,0,fCheckOverlaps);
  G4VSolid* lightGuideBotS = new G4Trd("Light Guide",pmtRadius,rpm8Thickness/2,pmtRadius,rpm8Width/2,lightGuideHeight/2);
  G4LogicalVolume* lightGuideBotLV = new G4LogicalVolume(lightGuideBotS,lightGuideMaterial,"Light Guide");
  new G4PVPlacement(0,rpm8Tran+G4ThreeVector(0,0,-lgZTran),lightGuideBotLV,"Bottom Light Guide",worldLV,false,0,fCheckOverlaps);

  // PMT Glass
  G4double pmtZTran = (rpm8Height+2*lightGuideHeight+pmtThickness)/2;  
  G4VSolid* pmtS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness/2,0,360*deg);
  pmtTopLV = new G4LogicalVolume(pmtS,pmtMaterial,"PMT Glass",0);
  pmtTopPV = new G4PVPlacement(0,rpm8Tran+G4ThreeVector(0,0,pmtZTran),pmtTopLV,"PMTGlass",worldLV,false,0,fCheckOverlaps);
  pmtBottomLV = new G4LogicalVolume(pmtS,pmtMaterial,"PMT Glass",0);
  pmtBottomPV = new G4PVPlacement(0,rpm8Tran+G4ThreeVector(0,0,-pmtZTran),pmtBottomLV,"PMTGlass",worldLV,false,0,fCheckOverlaps);
  
  G4VisAttributes* atb= new G4VisAttributes(G4Colour::Yellow());
  atb->SetForceSolid(true);
  lightGuideBotLV->SetVisAttributes(atb);
  lightGuideTopLV->SetVisAttributes(atb);
  pmtBottomLV->SetVisAttributes(atb);
  pmtTopLV->SetVisAttributes(atb);

  // Setting PMT Sensitive Detectors
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  pmtTopSD = new PMTSD("PMT/TopPMTSD","PMTTopHitCollection");
  pmtBottomSD = new PMTSD("PMT/BottomPMTSD","PMTBottomHitCollection");
  SDman->AddNewDetector(pmtTopSD);
  SDman->AddNewDetector(pmtBottomSD);
  pmtTopLV->SetSensitiveDetector(pmtTopSD);
  pmtBottomLV->SetSensitiveDetector(pmtBottomSD);

  // Setting Light Guide Sensitive Detectors
  lgTopSD = new PMTSD("LightGuide/TopPMTSD","LGTopHitCollection");
  lgBotSD = new PMTSD("LightGuide/BottomPMTSD","LGBotHitCollection");
  SDman->AddNewDetector(lgTopSD);
  SDman->AddNewDetector(lgBotSD);
  lightGuideTopLV->SetSensitiveDetector(lgTopSD);
  lightGuideBotLV->SetSensitiveDetector(lgBotSD);

  // Print Materials
  G4cout<<*(G4Material::GetMaterialTable());

  // Printing basic information about the geometry
  PrintParameters();


  // Return the worlds physical volume
  return worldPV;
}

/**
 * Constructs a Detector Slice
 */
G4LogicalVolume* DetectorConstruction::ConstructDetectorSlice(G4int copyNum){

  // Setting up the slice
  std::ostringstream osSlice;
  osSlice <<"Slice_"<<copyNum;
  std::string sliceName = osSlice.str();
  G4VSolid* sliceS = new G4Box("DetectorSlice",sliceThickness/2,rpm8Width/2,rpm8Height/2);
  G4LogicalVolume* sliceLV = new G4LogicalVolume(sliceS,detMountMaterial,sliceName);

  // Setting up the Absorber Solid Sheet
  absS.push_back(new G4Box("DetectorSheet",detectorThickness/2, rpm8Width/2,rpm8Height/2));
  absLV.push_back(new G4LogicalVolume(absS.back(),absMaterial,"Absorber"));
  absPV.push_back(new G4PVPlacement(0,G4ThreeVector(),absLV.back(),"Absorber",sliceLV,0,copyNum,fCheckOverlaps));

  G4VisAttributes* atb= new G4VisAttributes(G4Colour::Green());
  atb->SetForceSolid(true);
  absLV.back()->SetVisAttributes(atb);

  // Setting up the Absorber Sensitive Detector
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  std::ostringstream oss;
  oss <<"Absorber/AbsSD_"<<copyNum;
  std::string name = oss.str();
  oss.str("");
  oss.clear();
  oss<<"AbsHitCollection_"<<copyNum;
  AbsorberSD* absSD = new AbsorberSD(name,oss.str());
  SDman->AddNewDetector(absSD);
  absLV.back()->SetSensitiveDetector(absSD);

  G4double xTran = 0;
  if (mountThickness > 0){
    // Setting up the shapes
    xTran = detMountThickness;
    G4VSolid* mountS = new G4Box("Mounting",mountThickness/2,rpm8Width/2,rpm8Height/2);
    G4LogicalVolume* mountLV = new G4LogicalVolume(mountS,mountMaterial,"Optical Mounting");

    // Setting up the left coupling
    new G4PVPlacement(0,G4ThreeVector(-xTran,0,0),mountLV,"OpticalCoupling",sliceLV,0,-copyNum,fCheckOverlaps);

    // Setting up the right coupling
    new G4PVPlacement(0,G4ThreeVector(xTran,0,0),mountLV,"OpticalCoupling",sliceLV,0,copyNum,fCheckOverlaps);

    // TODO: Add in the optical surfaces
  }
  return sliceLV;
}
/**
 * Constructs the RPM8 Logical Volume
 *
 * @return a pointer to the RPM8 logical volume
 */
G4LogicalVolume* DetectorConstruction::ConstructRPM8(std::list<G4double> const& slices){

  /**
   * Setting up the RPM8 Volume
   */
  rpm8S = new G4Box("RPM8 footprint",rpm8Thickness/2,rpm8Width/2,rpm8Height/2);
  rpm8LV = new G4LogicalVolume(rpm8S,FindMaterial("G4_POLYETHYLENE"),"HDPE Moderator");

  // Itterating through, setting up the detector
  G4double sliceXPos = 0;
  G4int copyNum = 0;
  G4LogicalVolume* sliceLV;
  std::list<G4double>::const_iterator it;
  std::ostringstream oss;
  for(it = slices.begin(); it!=slices.end(); it++){
    oss <<"Slice_"<<copyNum;
    sliceXPos = *it;
    sliceLV = ConstructDetectorSlice(copyNum); 
    new G4PVPlacement(0,G4ThreeVector(sliceXPos-rpm8Thickness/2,0,0),sliceLV,oss.str(),rpm8LV,false,copyNum,fCheckOverlaps);
    copyNum ++;
    oss.str("");
    oss.flush();
  }

  // Setting up the visulaziation attributes
  G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
  atb->SetForceSolid(true);
  rpm8LV->SetVisAttributes(atb);
  // Returning the Logical Volume
  return rpm8LV;
}

/**
 * ConstructReflector
 *
 * Constructs the reflector around the detector assembly
 */
G4LogicalVolume* DetectorConstruction::ConstructReflector(){
  //TODO
  /*
     G4Tubs *refSide = new G4Tubs("refSide",gs20Radius,gs20Radius+refThickness,(refThickness+detectorThickness+mountThickness)/2,0,360*deg);
     G4Tubs *refTop = new G4Tubs("refTop",0,gs20Radius,refThickness/2,0,360*deg);
     refS = new G4UnionSolid("Reflector",refSide,refTop,0,G4ThreeVector(0,0,(detectorThickness+mountThickness)/2));
     refLV = new G4LogicalVolume(refS,refMaterial,"Reflector",0);
     zTran = (refThickness-mountThickness)/2;
     refPV = new G4PVPlacement(0,G4ThreeVector(0,0,zTran),refLV,"Reflector",worldLV,false,0,fCheckOverlaps);
     */
  return NULL;
}
/**
 * Constructs the Source Logical Volume
 *
 * The source configuration is specified in PNNL 18471
 * @return a pointer to the logical volume of the source which needs to 
 *  be placed in the Physical Volume
 */
G4LogicalVolume* DetectorConstruction::ConstructSource(){

  // HDPE Shell
  G4VSolid* sourceS = new G4Sphere("ModSrc",0,srcRadius+pbSrcThickness+modSrcThickness,0,360*deg,0,180*deg);
  sourceLV = new G4LogicalVolume(sourceS,FindMaterial("G4_POLYETHYLENE"),"HDEPE encased souce");

  // Lead Shell
  G4VSolid* pbSrcS = new G4Sphere("PbSrc",srcRadius,srcRadius+pbSrcThickness,0,360*deg,0,180*deg);
  G4LogicalVolume* pbSrcLV = new G4LogicalVolume(pbSrcS,FindMaterial("G4_Pb"),"Lead encasing Source");
  new G4PVPlacement(0,G4ThreeVector(),pbSrcLV,"Lead encasing source",sourceLV,false,0,fCheckOverlaps);

  // Cf252 source
  G4VSolid* cf252S = new G4Sphere("Cf252Source",0,srcRadius,0,360*deg, 0,180*deg);
  G4LogicalVolume* cf252LV = new G4LogicalVolume(cf252S,FindMaterial("G4_Pb"),"Cf252");
  new G4PVPlacement(0,G4ThreeVector(),cf252LV,"Cf252",sourceLV,false,0,fCheckOverlaps);

  // Setting Visulatization of the source
  G4VisAttributes* atb= new G4VisAttributes(G4Colour::Blue());
  atb->SetForceSolid(true);
  sourceLV->SetVisAttributes(atb);
  return sourceLV;
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
 * SetAbsorberThickness
 *
 * Sets the detector thickness
 */
void DetectorConstruction::SetAbsorberThickness(G4double val){
  detectorThickness = val;
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

  // Updating the engine
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RegionStore::GetInstance()->UpdateMaterialList(worldPV);
}
