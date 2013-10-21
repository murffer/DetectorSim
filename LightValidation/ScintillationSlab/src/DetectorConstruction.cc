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

PhotonDetSD* DetectorConstruction::mppcSD = NULL;

DetectorConstruction::DetectorConstruction()
 : physiWorld(NULL)
{

  detectorMessenger = new DetectorMessenger(this);
  materials = NULL;

  numOfCladLayers = 0;
 
  surfaceRoughness = 1;
 
  mirrorToggle = true;
  mirrorPolish = 1.;
  mirrorReflectivity = 1.;

  mppcPolish = 1.;
  mppcReflectivity = 0.;

  extrusionPolish = 1.;
  extrusionReflectivity = 1.;
 
  XYRatio = 1.0;

  wlsfiberZ     = 1.*m;
  wlsfiberRY  = 0.5*mm;
  wlsfiberOrigin = 0.0;
 
  mppcShape = "Circle";
  mppcHalfL = wlsfiberRY;
  mppcDist  = 0.00*mm;
  mppcTheta = 0.0*deg;
  mppcZ     = 0.05*mm;
 
  clrfiberZ  = mppcZ + 10.*nm ;
  mirrorZ    = 0.1*mm;

  barLength        = 1.*m;
  barBase          = 9.6*mm;
  holeRadius       = 0.9*mm;
  holeLength       = barLength;
  coatingThickness = 0.25*mm;
  coatingRadius    = 1.875*mm;

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
  //--------------------------------------------------
  // World
  //--------------------------------------------------

  G4VSolid* solidWorld = new G4Box("World", worldSizeX, worldSizeY, worldSizeZ);
  logicWorld = new G4LogicalVolume(solidWorld,FindMaterial("G4_AIR"),"World");
  physiWorld = new G4PVPlacement(0,G4ThreeVector(), logicWorld, "World", 0, false, 0);


  G4OpticalSurface* TiO2Surface = new G4OpticalSurface("TiO2Surface", glisur,  ground, dielectric_metal, extrusionPolish);
  G4MaterialPropertiesTable* TiO2SurfaceProperty = new G4MaterialPropertiesTable();

  G4double p_TiO2[2] = {2.00*eV, 3.47*eV};
  G4double refl_TiO2[2] = {extrusionReflectivity,extrusionReflectivity};
  G4double effi_TiO2[2] = {0, 0};
  TiO2SurfaceProperty -> AddProperty("REFLECTIVITY",p_TiO2,refl_TiO2,2);
  TiO2SurfaceProperty -> AddProperty("EFFICIENCY",p_TiO2,effi_TiO2,2);
  TiO2Surface -> SetMaterialPropertiesTable(TiO2SurfaceProperty);

  new G4PVPlacement(0, G4ThreeVector(),logicExtrusion,"Extrusion", logicWorld, false,0);

  new G4LogicalSkinSurface("TiO2Surface",logicExtrusion,TiO2Surface);

  //--------------------------------------------------
  // Scintillator
  //--------------------------------------------------
    G4VSolid* solidScintillator = new G4Box("Scintillator",scintX/2,scintY/2,scintZ/2);
  G4LogicalVolume* logicScintillator = new G4LogicalVolume(solidScintillator,FindMaterial("Polystyrene"),"Scintillator");
  new G4PVPlacement(0, G4ThreeVector(), logicScintillator, "Scintillator", logicWorld, false, 0);
    
    // Setting up the cladding
    G4Box *solidClad1, *solidClad2;
    G4LogicalVolume   *logicClad1, *logicClad2;
    G4VPhysicalVolume *physiClad1, *physiClad2;
  
    // Determine the number of cladding layers to be built
    switch ( numOfCladLayers ) {
        case 2:
            //--------------------------------------------------
            // Cladding 2
            //--------------------------------------------------
            G4double t = clad1Thickness+clad2Thickness;
            G4double zOrig = t/2;
            solidClad2 = new G4Box("Clad2",(scintX+t)/2,(scintY+t)/2,(scintZ+t)/2);
            logicClad2  = new G4LogicalVolume(solidClad2,FindMaterial("FPethylene"), "Clad2");
            physiClad2 = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrig), logicClad2,"Clad2", logicPlacement, false,0);
            
            // Place the rough surface only if needed
            if (OpSurface) {
                new G4LogicalBorderSurface("surfaceClad2Out",physiClad2,physiPlacement,OpSurface);
                new G4LogicalBorderSurface("surfaceClad2In",  physiPlacement,physiClad2,OpSurface);
            }
            
            logicPlacement = logicClad2;
            physiPlacement = physiClad2;
        case 1:
        
            
            //--------------------------------------------------
            // Cladding 1
            //--------------------------------------------------
            
            G4double t = clad1Thickness;
            G4double zOrig = t/2;
            solidClad1 = new G4Box("Clad1",(scintX+t)/2,(scintY+t)/2,scintZ/2+t);
            logicClad1 = new G4LogicalVolume(solidClad1,FindMaterial("Pethylene"),"Clad1");
            physiClad1 = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrg), logicClad1, "Clad1", logicPlacement,false,0);
            
            // Place the rough surface only if needed
            if (OpSurface) {
                new G4LogicalBorderSurface("surfaceClad1Out",physiClad1,physiPlacement,OpSurface);
                new G4LogicalBorderSurface("surfaceClad1In",physiPlacement,physiClad1,OpSurface);
            }
            
            logicPlacement = logicClad1;
            physiPlacement = physiClad1;
            
        case 0:
            
            //--------------------------------------------------
            // WLS Fiber
            //--------------------------------------------------
            
            G4double t = wlsThickenss;
            G4double zOrig = t/2;
            G4Box* solidWLS = new G4Box("WLS Slab",(scintX+t)/2,(scintY+t)/2,scintZ/2+t);
            G4LogicalVolume*   logicWLS = new G4LogicalVolume(solidWLS,FindMaterial("PMMA"),"WLS Slab");
            G4VPhysicalVolume* physiWLS = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrig),logicWLS,"WLS Slab",logicPlacement,false,0);
            
            logicWLS->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
            
            // Place the rough surface only if needed
            if (OpSurface) {
                new G4LogicalBorderSurface("surfaceWLSOut",physiWLS,physiPlacement,OpSurface);
                new G4LogicalBorderSurface("surfaceWLSIn",physiPlacement,physiWLS, OpSurface);
            }
        default:
            G4cerr<<"No cladding is material is selected"<<G4endl;
    }
    
    //--------------------------------------------------
    // Light Guide
    //--------------------------------------------------
    
    //--------------------------------------------------
    // PhotonDet (Sensitive Detector)
    //--------------------------------------------------
    
    // Physical Construction
    G4double zOrig = scintZ+pmtHalfL/2;
    G4VSolid* solidPhotonDet = new G4Box("PhotonDet",scintX,scintY,pmtHalfL/2);
    G4LogicalVolume*   logicPhotonDet = new G4LogicalVolume(solidPhotonDet, FindMaterial("G4_Pyrex_Glass"), "PhotonDet");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,zOrig), logicPhotonDet, "PhotonDet", logicClrfiber, false, 0);
    
    // PhotonDet Surface Properties
    G4OpticalSurface* PhotonDetSurface = new G4OpticalSurface("PhotonDetSurface", glisur, ground,dielectric_metal,mppcPolish);
    G4MaterialPropertiesTable* PhotonDetSurfaceProperty =new G4MaterialPropertiesTable();
    
    G4double p_mppc[2] = {2.00*eV, 3.47*eV};
    G4double refl_mppc[2] = {mppcReflectivity,mppcReflectivity};
    G4double effi_mppc[2] = {1, 1};
    
    PhotonDetSurfaceProperty -> AddProperty("REFLECTIVITY",p_mppc,refl_mppc,2);
    PhotonDetSurfaceProperty -> AddProperty("EFFICIENCY",p_mppc,effi_mppc,2);
    
    PhotonDetSurface -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
    new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet,PhotonDetSurface);
    if (!mppcSD) {
        G4String mppcSDName = "/PhotonDet";
        mppcSD = new PhotonDetSD(mppcSDName);
        
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        SDman->AddNewDetector(mppcSD);
    }
    
    // Setting the detector to be sensitive
    logicPhotonDet->SetSensitiveDetector(mppcSD);
    
  return physiWorld;
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
  wlsfiberRX  = XYRatio * wlsfiberRY;

  clad1RX = wlsfiberRX + 0.03*wlsfiberRX;
  clad1RY = wlsfiberRY + 0.03*wlsfiberRY;
  clad1Z  = wlsfiberZ;

  clad2RX = clad1RX + 0.03*wlsfiberRX;
  clad2RY = clad1RY + 0.03*wlsfiberRY;
  clad2Z  = wlsfiberZ;

  worldSizeX = clad2RX   + mppcDist + mppcHalfL + 1.*cm;
  worldSizeY = clad2RY   + mppcDist + mppcHalfL + 1.*cm;
  worldSizeZ = wlsfiberZ + mppcDist + mppcHalfL + 1.*cm;
 
  coupleRX = worldSizeX;
  coupleRY = worldSizeY;
  coupleZ  = (worldSizeZ - wlsfiberZ) / 2;
 
  clrfiberHalfL = mppcHalfL;
 
  mirrorRmax = clad2RY;
 
  coupleOrigin = wlsfiberOrigin + wlsfiberZ + coupleZ; 
  mirrorOrigin = wlsfiberOrigin - wlsfiberZ - mirrorZ; 
  mppcOriginX  = std::sin(mppcTheta) * (mppcDist + clrfiberZ);
  mppcOriginZ  = -coupleZ + std::cos(mppcTheta) * (mppcDist + clrfiberZ);
}


// Set the number of claddings
// Pre: 0 <= num <= 2
void DetectorConstruction::SetNumberOfCladding(G4int num)
{
  numOfCladLayers = num;
}

// Set the TOTAL length of the  fiber
void DetectorConstruction::SetLength (G4double length)
{
  wlsfiberZ = length;
}

// Set the Y radius of  fiber
void DetectorConstruction::SetRadius (G4double radius)
{
  wlsfiberRY = radius;
}

// Set the Y radius of Cladding 1
void DetectorConstruction::SetClad1Radius (G4double radius)
{
  clad1RY = radius;
}

// Set the Y radius of Cladding 2
void DetectorConstruction::SetClad2Radius (G4double radius)
{
  clad2RY = radius;
}

// Set the half length of the PhotonDet detector
// The half length will be the radius if PhotonDet is circular
void DetectorConstruction::SetPhotonDetHalfLength(G4double halfL)
{
  mppcHalfL = halfL;
}

// Set the distance between fiber end and PhotonDet
void DetectorConstruction::SetGap (G4double gap) { mppcDist = gap; }

// Set the Aligment of PhotonDet with respect to the z axis
// If theta is 0 deg, then the detector is perfectly aligned
// PhotonDet will be deviated by theta from z axis
// facing towards the center of the fiber
void DetectorConstruction::SetPhotonDetAlignment(G4double theta)
{
  mppcTheta = theta;
}

// Set the Surface Roughness between Cladding 1 and  fiber
// Pre: 0 < roughness <= 1
void DetectorConstruction::SetSurfaceRoughness(G4double roughness)
{
  surfaceRoughness = roughness;
}


// Set the Polish of the PhotonDet, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
void DetectorConstruction::SetPhotonDetPolish(G4double polish)
{
  mppcPolish = polish;
}

// Set the Reflectivity of the PhotonDet, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
void DetectorConstruction::SetPhotonDetReflectivity(G4double reflectivity)
{
  mppcReflectivity = reflectivity;
}

// Toggle to place the mirror or not at one end (-z end) of the fiber
// True means place the mirror, false means otherwise
void DetectorConstruction::SetMirror(G4bool flag) { mirrorToggle = flag; }

// Set the ratio of the x and y radius of the ellipse (x/y)
// a ratio of 1 would produce a circle
void DetectorConstruction::SetXYRatio(G4double r) { XYRatio = r; }

// Set the length of the scintillator bar
void DetectorConstruction::SetBarLength (G4double length)
{
  barLength = length;
}

// Set the side of the scintillator bar
void DetectorConstruction::SetBarBase (G4double side)
{
  barBase = side;
}

// Set thickness of the coating on the bars
void DetectorConstruction::SetCoatingThickness (G4double thick)
{
  coatingThickness = thick;
}

// Set inner radius of the corner bar coating
void DetectorConstruction::SetCoatingRadius (G4double radius)
{
  coatingRadius = radius;
}

G4double DetectorConstruction::GetLength() { return wlsfiberZ; }

G4double DetectorConstruction::GetBarLength() { return barLength; }
G4double DetectorConstruction::GetBarBase() { return barBase; }
G4double DetectorConstruction::GetHoleRadius() { return holeRadius; }
G4double DetectorConstruction::GetHoleLength() { return holeLength; }
G4double DetectorConstruction::GetFiberRadius() { return GetFiberRMax(); }
G4double DetectorConstruction::GetCoatingThickness() 
                                                  { return coatingThickness; }
G4double DetectorConstruction::GetCoatingRadius() { return coatingRadius; }

/**
 Gets the end of the length of the slab
 @returns the end of the length of the slab
 */
G4double DetectorConstruction::GetEnd()
{
  return wlsfiberOrigin + wlsfiberZ;
}

G4double DetectorConstruction::GetFiberRMax()
{
  if (numOfCladLayers == 2) return clad2RY;
  if (numOfCladLayers == 1) return clad1RY;
  return wlsfiberRY;
}

G4double DetectorConstruction::GetSurfaceRoughness()
{
  return surfaceRoughness;
}
/**
 @brief Returns true if the scintillator slab construction is ideal.
 An ideal scintillator slab has a surfaceRoughness of 1 with perfect reflectivity and a mirror polish of 1
 @returns True is it is ideal, false otherwise
 */
G4bool DetectorConstruction::IsPerfectFiber()
{ 
  return     surfaceRoughness == 1. && XYRatio == 1.
             && (!mirrorToggle    || 
             (mirrorPolish    == 1. && mirrorReflectivity == 1.));
}

G4Material* DetectorConstruction::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}
