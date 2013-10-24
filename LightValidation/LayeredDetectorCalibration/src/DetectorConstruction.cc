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
    
    // Physical World
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
    <<"\n\tThickness (x): "<<G4BestUnit(pmmaThickness,"Length")
    <<"\n\tWidth (y):     "<<G4BestUnit(pmmaWidth,"Length")
    <<"\n\tHeight (z):    "<<G4BestUnit(pmmaHeight,"Length")
    <<"\n--> Detector Slice (M A M):"
    <<"\n\tThickness: "<<G4BestUnit(sliceThickness,"Length")
    <<"\n\t--> Abosrber (A):"
    <<"\n\t\tMaterial (1x):  "<<absMaterial->GetName()
    <<"\n\t\tThickness: "<<G4BestUnit(detectorThickness,"Length")
    <<"\n\t--> Detector Mounting (M):"
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
    
    // Geometry parameters
    pmtRadius = 2.54*cm;
    pmtThickness = 5*mm;
    
    mountThickness = 100*um;
    refThickness = 3.33*mm;
    
    pmmaWidth = 10.16*cm;
    pmmaHeight = 15.24*cm;
    pmmaThickness = 6*cm;

    detectorThickness = 100*um;
    lightGuideHeight = 5.08*cm;
    
    absMaterial = FindMaterial("EJ426");
    pmtMaterial = FindMaterial("BK7");
    refMaterial = FindMaterial("G4_TEFLON");
    mountMaterial = FindMaterial("Silicone");
    lightGuideMaterial = FindMaterial("G4_PLEXIGLASS");
    
    /* World */
    worldS = new G4Box("World",25*cm,25*cm,25*cm);
    worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);
    {
        G4VisAttributes* atb= new G4VisAttributes(G4Colour::Yellow());
        atb->SetVisibility(false);
        worldLV->SetVisAttributes(atb);
    }
    
    /* Teflon Cladding around PMMA and Detector Slices */
    cladS = new G4Box("Reflector_Clading",(pmmaThickness+refThickness)/2,(pmmaWidth+refThickness)/2,(pmmaHeight+refThickness)/2);
    cladLV = new G4LogicalVolume(cladS,cladMaterial,"Reflector");
    cladPV = new G4PVPlacement(0,G4ThreeVector(-refThickness,0,0),cladLV,"Reflector",worldLV,false,0,fCheckOverlaps);
    
    /* Creating and Positing the PMMA and Detector
     * 
     * The Detector slices are positioned inside of the PMMA, and contains the detector and mounting
     * slices. Each slice is positioned inside some optical grease (silcone).
     */
    sliceThickness = detectorThickness+2*mountThickness;
    std::list<G4double> slices;
    slices.push_back(2*cm);
    slices.push_back(4*cm);
    G4cout<<"Creating Detector Slices"<<G4endl;
    pmmaLV = ConstructPMMA(slices);
    pmmaPV = new G4PVPlacement(0,G4ThreeVector(),pmmaLV,"PMMA  Detector",refLV,false,0,fCheckOverlaps);
    
    /**
     * Setting up the Light Guides and PMT's
     */
    G4double lgZTran = pmmaHeight/2+lightGuideHeight/2;
    G4VSolid* lightGuideS = new G4Trd("Light Guide",pmmaThickness/2,pmtRadius,pmmaWidth/2,pmtRadius,lightGuideHeight/2);
    G4LogicalVolume* lightGuideLV = new G4LogicalVolume(lightGuideS,lightGuideMaterial,"Light Guide");
    new G4PVPlacement(0,G4ThreeVector(0,0,lgZTran),lightGuideLV,"Top Light Guide",worldLV,false,0,fCheckOverlaps);
    
    // PMT Glass
    G4double pmtZTran = (pmmaHeight+2*lightGuideHeight+pmtThickness)/2;
    G4VSolid* pmtS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness/2,0,360*deg);
    pmtLV = new G4LogicalVolume(pmtS,pmtMaterial,"PMT Glass",0);
    pmtPV = new G4PVPlacement(0,G4ThreeVector(0,0,pmtZTran),pmtLV,"PMTGlass",worldLV,false,0,fCheckOverlaps);

    
    G4VisAttributes* atb= new G4VisAttributes(G4Colour::Yellow());
    atb->SetForceSolid(true);
    lightGuideLV->SetVisAttributes(atb);
    pmtLV->SetVisAttributes(atb);

    
    // Setting PMT Sensitive Detectors
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    pmtSD = new PMTSD("PMT/PMTSD","PMTTopHitCollection");
    SDman->AddNewDetector(pmtSD);
    pmtLV->SetSensitiveDetector(pmtSD);

    
    // Print Materials
    G4cout<<*(G4Material::GetMaterialTable());
    
    // Printing basic information about the geometry
    PrintParameters();
    
    
    // Return the worlds physical volume
    return worldPV;
}

/**
 * Constructs a Detector Slice.
 * 
 * A detector slice consists of the mounting material (silcone) 
 * which is the larger, parent volume of the detector slab.
 *
 * @return - a pointer to the mounting slice logical volume 
 *      which contains the detector slice.
 */
G4LogicalVolume* DetectorConstruction::ConstructDetectorSlice(G4int copyNum){
    
    // Setting up the slice
    std::ostringstream osSlice;
    osSlice <<"Slice_"<<copyNum;
    std::string sliceName = osSlice.str();
    
    G4VSolid* sliceS= new G4Box("Mounting",sliceThickness/2,pmmaWidth/2,pmmaHeight/2);
    G4LogicalVolume* sliceLV = new G4LogicalVolume(sliceS,mountMaterial,sliceName);
 

    // Setting up the Absorber Solid Sheet
    absS.push_back(new G4Box("DetectorSheet",detectorThickness/2, pmmaWidth/2,pmmaHeight/2));
    absLV.push_back(new G4LogicalVolume(absS.back(),absMaterial,"Absorber"));
    absPV.push_back(new G4PVPlacement(0,G4ThreeVector(),absLV.back(),"Absorber",sliceLV,0,copyNum,fCheckOverlaps));
    
    G4VisAttributes* atb= new G4VisAttributes(G4Colour::Green());
    atb->SetForceSolid(true);
    absLV.back()->SetVisAttributes(atb);
    
    return sliceLV;
}
/**
 * Constructs the RPM8 Logical Volume
 *
 * @return a pointer to the RPM8 logical volume
 */
G4LogicalVolume* DetectorConstruction::ConstructPMMA(std::list<G4double> const& slices){
    
    /**
     * Setting up the RPM8 Volume
     */
    pmmaS = new G4Box("RPM8 footprint",pmmaThickness/2,pmmaWidth/2,pmmaHeight/2);
    pmmaLV = new G4LogicalVolume(pmmaS,FindMaterial("G4_POLYETHYLENE"),"HDPE Moderator");
    
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
        new G4PVPlacement(0,G4ThreeVector(sliceXPos-pmmaThickness/2,0,0),sliceLV,oss.str(),pmmaLV,false,copyNum,fCheckOverlaps);
        copyNum ++;
        oss.str("");
        oss.flush();
    }
    
    // Setting up the visulaziation attributes
    G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
    atb->SetForceSolid(true);
    pmmaLV->SetVisAttributes(atb);
    // Returning the Logical Volume
    return pmmaLV;
}


/**
 * SetsOpticalSurfaces
 *
 * Sets the optical surfaces for the detector volumes
 */
void DetectorConstruction::SetOpticalSurfaces(){
    // Reflector (Teflon) Optical Surface
    /*
    G4OpticalSurface *refOpSurface = new G4OpticalSurface("Reflector Surface");
    refOpSurface->SetType(dielectric_LUT);
    refOpSurface->SetModel(LUT);
    refOpSurface->SetFinish(polishedteflonair);
    G4LogicalSkinSurface *refLogSurface = new G4LogicalSkinSurface("Reflector Logical Skin Surface",refLV,refOpSurface);
    
    // Detector Mounting and Silicon Grease Optical Surface (TODO)
    // This may not be necessary as we can almost assume perfect coupling between the GS20 and the PMT due to
    // the optical grease.
     */
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
