#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
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
        absThickness = 5*cm;	        // Thickness of Absorber
        gapThickness = 1*cm;            // Thickness of Gap 
        oRadius  = 2.54*cm;		        // Outer Radius of Detector
        iRadius = 0.*cm;				// Inner radious of  Detector
        startAngle = 0.*deg;
        spanAngle = 360.*deg;

        // Compute parameters
        ComputeParameters();

        // Define materials 
        DefineMaterials();
        SetAbsorberMaterial("EJ426HD2");
        SetGapMaterial("G4_PLEXIGLASS");

        // Create commands for interactive defiantions of the calorimeter
        detectorMessenger = new DetectorMessenger(this);
    }

DetectorConstruction::~DetectorConstruction(){

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


void DetectorConstruction::DefineMaterials()
{
    G4String name, symbol;             // a=mass of a mole;
    G4double a, z, density;            // z=mean number of protons;  
    G4int iz, n;                       // iz=nb of protons  in an isotope; 
    // n=nb of nucleons in an isotope;
    G4int nAtoms;
    G4int nComponents;

    // NIST Material Manager
    G4NistManager* nistManager = G4NistManager::Instance();
    G4bool fromIsotopes = true;

    // Getting Elements
    G4Element* eH = nistManager->FindOrBuildElement("H",fromIsotopes);
    G4Element* eC = nistManager->FindOrBuildElement("C",fromIsotopes);
    G4Element* eO = nistManager->FindOrBuildElement("O",fromIsotopes);
    G4Element* eN = nistManager->FindOrBuildElement("N",fromIsotopes);
    G4Element* eF = nistManager->FindOrBuildElement("F",fromIsotopes);
    G4Element* eS = nistManager->FindOrBuildElement("S",fromIsotopes);
    G4Element* eZn = nistManager->FindOrBuildElement("Zn",fromIsotopes);

    // defining enriched Lithium 6
    G4double a6Li = 6.015*g/mole;	// Molar Masses (Wolfram Alpha)
    G4double a7Li = 7.016*g/mole;
    G4double enrichement = 110.815*perCent;
    G4double abundance6Li = enrichement*a6Li/a7Li;		// Relative Abudadance
    G4double abundance7Li = 1-abundance6Li;

    G4Isotope* Li6 = new G4Isotope(name="Li6", iz=3, n=6, a6Li);
    G4Isotope* Li7 = new G4Isotope(name="Li7", iz=3, n=7, a7Li);
    G4Element* enrichLi  = new G4Element(name="enriched Lithium", symbol="Li", nComponents=2);
    enrichLi->AddIsotope(Li6, abundance6Li);
    enrichLi->AddIsotope(Li7, abundance7Li);

    // Defining 6LiF Absorber
    G4Material* LiAbsorber = new G4Material("6LiF",density=1.0*g/cm3,nComponents=2);
    LiAbsorber->AddElement(enrichLi,nAtoms=1);
    LiAbsorber->AddElement(eF,nAtoms=1);

    // PPO C15H11NO
    G4Material* PPO = new G4Material("PPO",density=1.1*g/cm3,nComponents=4,kStateSolid);
    PPO->AddElement(eC,nAtoms=15);
    PPO->AddElement(eH,nAtoms=11);
    PPO->AddElement(eO,nAtoms=1);
    PPO->AddElement(eN,nAtoms=1);

    // POPOP C24H15N2O2
    G4Material* POPOP = new G4Material("POPOP",density=1.1*g/cm3,nComponents=4,kStateSolid);
    POPOP->AddElement(eC,nAtoms=24);
    POPOP->AddElement(eH,nAtoms=15);
    POPOP->AddElement(eO,nAtoms=2);
    POPOP->AddElement(eN,nAtoms=2);

    // Scintillant
    G4double fractionPPO = 46./(46.+1.36);		// Scintillant is in the ratio of 46 g PPO to 1.36 g POPOP
    G4Material* scintillant = new G4Material("PPO/POPOP",density=1.1*g/cm3,nComponents=2,kStateSolid);
    scintillant->AddMaterial(PPO,fractionPPO);
    scintillant->AddMaterial(POPOP,1-fractionPPO);

    // Polymer PS Based Detector
    G4double fractionPolymer = 0.85;
    G4double fractionScintillant = 0.05;
    G4double fractionAbsorber = 0.10;
    G4Material* psDetector = new G4Material("PS_Detector",density=1.1*g/cm3,nComponents=3,kStateSolid);
    psDetector->AddMaterial(nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes),fractionPolymer);
    psDetector->AddMaterial(scintillant,fractionScintillant);
    psDetector->AddMaterial(LiAbsorber,fractionAbsorber);
    
    // Defining EJ426 HD2
    G4double massFraction;
    G4Material* EJ426HD2 = new G4Material("EJ426HD2",density=4.1*g/cm3,nComponents=4);
    EJ426HD2->AddElement(enrichLi,massFraction=0.081);
    EJ426HD2->AddElement(eF,massFraction=0.253);
    EJ426HD2->AddElement(eZn,massFraction=0.447);
    EJ426HD2->AddElement(eS,massFraction=0.219);

    // Vacuum
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

    nistManager->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_AIR",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_WATER",fromIsotopes);
   
   // Print materials
    // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Default Material
    defaultMaterial = G4Material::GetMaterial("Galactic");
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
    //atb->SetForceSolid(true);
    absLV->SetVisAttributes(atb);}

    // Setting the Visualization attributes for the Calorimeter 
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
    //atb->SetForceWireframe(true);
    //atb->SetForceSolid(true);
    gapLV->SetVisAttributes(atb);}

    // Setting the Layers to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
   // atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldLV->SetVisAttributes(atb);}
    
    // Setting the World to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
    //atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldPV->GetLogicalVolume()->SetVisAttributes(atb);}

}

/**
 * SetAbsorberMaterial
 *
 * Set's the absorber material, which contains the neutron absorber
 */
void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice){
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
    if (pttoMaterial) absMaterial = pttoMaterial;
}
/**
 * SetGapMaterial
 *
 * Set's the gap material.
 */
void DetectorConstruction::SetGapMaterial(G4String materialChoice){
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
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
    
    absLV->SetSensitiveDetector(caloSD);

    // Setting the Maximum Step Size
    G4double maxStep = 0.01*absThickness;
    absLV->SetUserLimits(new G4UserLimits(maxStep));

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
