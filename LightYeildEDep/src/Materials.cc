#include "Materials.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

/**
 * Constructor for the Optical Materials
 *
 * The optical materials are implemented as a static class
 */
Materials::Materials(){
    // Getting the NIST Material Manager
    nistMan = G4NistManager::Instance();
    nistMan->SetVerbose(0);
    
    // Creating my custom materials
    G4cout<<"Materials - Creating the materials"<<G4endl;
    CreateMaterials();
    SetOpticalPropertiesPS();
    SetOpticalPropertiesEJ200();
      SetOpticalPropertiesGS20();
      SetOpticalPropertiesEJ426();
    G4cout<<"Materials - Created all of the materials"<<G4endl;
}

Materials::~Materials(){
    delete EJ200;
    delete psDet;
    delete EJ426;
    delete GS20;
}
/**
 * Setting the Singleton Class
 */
Materials* Materials::instance=0;
/**
 * Instance of the Singleton Class
 *
 * @return the instance of the Singleton Materials
 */
Materials* Materials::GetInstance(){
    if (instance ==0){
        instance = new Materials();
    }
    return instance;
}

/**
 * Searches for the material in the material manager. If the material is not
 * found in the material manager or can not be built, and exception is issued.
 * These are optical materials, implying that they have optical properities
 * ascoated with the material.
 *
 * @param material Material Name
 * @return A pointer to the material
 */
G4Material* Materials::GetMaterial(const G4String material){
    
    // Trying to build the material
    G4Material* mat = nistMan->FindOrBuildMaterial(material);
    if (!mat) mat = G4Material::GetMaterial(material);
    
    // Exceptional error if it isn't found
    if(!mat) {
        std::ostringstream o;
        o<<"Material "<<material<<" not found.";
        G4Exception("Materials::GetMaterial","",FatalException,o.str().c_str());
    }
    return mat;
}

/**
 * Creates the optical materials
 */
void Materials::CreateMaterials(){
    G4double density;
    std::vector<G4int> natoms;
    std::vector<G4double> fractionMass;
    std::vector<G4String> elements;
    
    //-----------------------------------------------------------------------
    // EJ200
    // EJ200 was assumed to be PVT based
    //-----------------------------------------------------------------------
    EJ200 = nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    
    //--------------------------------------------------
    // Enriched LiF
    //--------------------------------------------------
    G4Isotope* Li6 = new G4Isotope("Li6",3,6,6.015*g/mole);
    G4Isotope* Li7 = new G4Isotope("Li7",3,7,7.015*g/mole);
    G4Element* enrichLi  = new G4Element("enriched Lithium","Li",2);
    enrichLi->AddIsotope(Li6,0.95*perCent);
    enrichLi->AddIsotope(Li7,0.05*perCent);
    
    G4Material* LiF = new G4Material("6LiF",density=2.64*g/cm3,2);
    LiF->AddElement(enrichLi,1);
    LiF->AddElement(nistMan->FindOrBuildElement("F"),1);
    
    //--------------------------------------------------
    // LiF Polystyrene
    //--------------------------------------------------
    G4Material* ps = nistMan->FindOrBuildMaterial("G4_POLYSTYRENE")
    psDet = new G4Material("PSLiF",1.1*g/cm3,2,kStateSolid);
    psDet->AddMaterial(ps,90*perCent);
    psDet->AddMaterial(LiF,10*perCent);
    
    //-----------------------------------------------------------------------
    // GS20
    // GS20 is (by mass fraction):
    //  56% SiO2, 4% MgO,  18% Al2O3 18% Li2O, and 4% Ce2O3
    //-----------------------------------------------------------------------
    G4Material* LiOxide = new G4Material("6LiO2",density=2.013*g/cm3,2);
    LiOxide->AddElement(enrichLi,2);
    LiOxide->AddElement(nistMan->FindOrBuildElement("O"),1);
    
    G4Material* SiO2 = nistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* MgO = nistMan->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
    G4Material* Al2O3 = nistMan->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
    
    elements.push_back("Ce");   natoms.push_back(2);
    elements.push_back("O");    natoms.push_back(3);
    density = 6.2*g/cm3;
    G4Material* Ce2O3 = nistMan->ConstructNewMaterial("Ce2O3",elements,natoms,density);
    elements.clear();           natoms.clear();
    
    density = 2.5*g/cm3;
    GS20 = new G4Material("GS20",density,5,kStateSolid);
    GS20->AddMaterial(SiO2,56*perCent);
    GS20->AddMaterial(MgO,4*perCent);
    GS20->AddMaterial(Al2O3,18*perCent);
    GS20->AddMaterial(LiOxide,18*perCent);
    GS20->AddMaterial(Ce2O3,4*perCent);
    
    //----------------------------------------------------------------------
    // EJ426 (LiF/ZnS(Ag)). See pg. 114 of Urffer II Labnote book
    //----------------------------------------------------------------------
    EJ426 = new G4Material("EJ426",density=4.1*g/cm3,4);
    EJ426->AddElement(enrichLi,0.081);
    EJ426->AddMaterial(nistMan->FindOrBuildMaterial("G4_F"),0.253);
    EJ426->AddMaterial(nistMan->FindOrBuildMaterial("G4_Zn"),0.447);
    EJ426->AddMaterial(nistMan->FindOrBuildMaterial("G4_S"),0.219);
}
/**
 * Sets the optical properties of EJ426
 *
 * EJ426 is LiF:ZnS(Ag), a commerical scintillator
 * Data Sources:
 *  Emission spectra is from EJ-426 data sheet
 *  Absorbition length is from assuing 10% transmittance at 100 microns
 *  Refractive index is from bicron, www.hep.ph.ic.uk/fets/pepperpot/docs+papers/zns_602.pdf (assumed to be flat)
 *  Light yield from Eijk
 */
void Materials::SetOpticalPropertiesEJ426(){
    // Refrative Index
    const G4int nRINDEX = 2;
    G4double photonEnergyRINDEX[nRINDEX] = {3.5*eV,2.75*eV};
    G4double RefractiveIndexEJ426[nRINDEX]= {2.36,2.36};
    
    // Absorbition Length
    const G4int nABS=2;
    G4double photonEnergyABS[nABS] = {3.5*eV,2.75*eV};
    G4double AbsLengthEJ426[nABS]={40*um,40*um};
    
    // Setting sctintillation to be the emission spectra
    const G4int nEM = 10;
    G4double photonEnergyEM[nEM] =
    {3.10*eV,2.95*eV,2.82*eV,2.76*eV,2.70*eV,
        2.58*eV,2.48*eV,2.38*eV,2.30*eV,2.14*eV};
    G4double emEJ426[nEM]=
    {0,0.0519,0.2208,0.2597,0.2338,
        0.1429,0.0519,0.0260,0.0130,0.0};
    
    // Creating the materials property table and adding entries into properties table
    G4MaterialPropertiesTable* MPTEJ426 = new G4MaterialPropertiesTable();
    MPTEJ426->AddProperty("RINDEX",photonEnergyRINDEX,RefractiveIndexEJ426,nRINDEX);
    MPTEJ426->AddProperty("ABSLENGTH",photonEnergyABS,AbsLengthEJ426,nABS);
    // Setting  Scintillation Properties
    MPTEJ426->AddProperty("FASTCOMPONENT",photonEnergyEM,emEJ426,nEM);
    MPTEJ426->AddConstProperty("FASTTIMECONSTANT",200*ns);
    MPTEJ426->AddConstProperty("SCINTILLATIONYIELD", 72000*MeV);
    MPTEJ426->AddConstProperty("YIELDRATIO", 1.0);
    MPTEJ426->AddConstProperty("RESOLUTIONSCALE", 1.0);
    MPTEJ426->DumpTable();
    EJ426->SetMaterialPropertiesTable(MPTEJ426);
}

/**
 * Sets the optical properties of GS20
 *
 * Data Sources:
 *  Assumed that the index of reflection and absorbition length are similar to BK7
 */
void Materials::SetOpticalPropertiesGS20(){
    
    // Index of Reflection (146 nm to 1570 nm)
    const G4int nRINDEX = 13;
    G4double photonEnergyRINDEX[nRINDEX] =
    {8.5506*eV,4.7232*eV,3.2627*eV,2.4921*eV,2.0160*eV,
        1.6926*eV,1.4586*eV,1.2815*eV,1.1427*eV,1.0311*eV,
        0.9393*eV,0.8625*eV,0.7973*eV};
    G4double RefractiveIndexGlass[nRINDEX]=
    {1.6508,1.5266,1.4980,1.4872,1.4819,
        1.4790,1.4772,1.4760,1.4752,1.4746,
        1.4742,1.4738,1.4736};
    
    // Absorbition Length
    const G4int nABS=2;
    G4double photonEnergyABS[nABS] = {3.5*eV,1.75*eV};
    G4double AbsLengthGlass[nABS] = {70*cm, 70*cm};
    
    // Setting sctintillation to be the emission spectra
    const G4int nEM = 6;
    G4double photonEnergyEM[nEM] = {3.8,3.5,3.1,2.8,2.5};
    G4double emGS20[nEM]={0,0.19,0.37,0.32,0.10,0.02};
	
    // Creating the materials property table and adding entries into properties table
    G4MaterialPropertiesTable* MPTGS20 = new G4MaterialPropertiesTable();
    MPTGS20->AddProperty("RINDEX",photonEnergyRINDEX,RefractiveIndexGlass,nRINDEX);
    MPTGS20->AddProperty("ABSLENGTH",photonEnergyABS,AbsLengthGlass,nABS);
    // Setting  Scintillation Properties
    MPTGS20->AddProperty("FASTCOMPONENT",photonEnergyEM,emGS20,nEM);
    MPTGS20->AddConstProperty("FASTTIMECONSTANT",50*ns);      //
    MPTGS20->AddConstProperty("SCINTILLATIONYIELD", 3600*MeV);
    MPTGS20->AddConstProperty("YIELDRATIO", 1.0);
    MPTGS20->AddConstProperty("RESOLUTIONSCALE", 1.0);
    GS20->SetMaterialPropertiesTable(MPTGS20);
    GS20->GetIonisation()->SetBirksConstant(0.0052*mm/MeV);
}

/**
 * Sets the Optical Properties for Polystyrene
 *
 */
void Materials::SetOpticalPropertiesPS(){
    //--------------------------------------------------
    //  Polystyrene
    //--------------------------------------------------
    
    const G4int nEntries = 50;
    
    G4double PhotonEnergy[nEntries] =
    {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
        2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
        2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
        2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
        2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
        2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
        2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
        3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
        3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
        3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
    G4double RefractiveIndexPS[nEntries] =
    { 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};
    
    G4double AbsPS[nEntries] =
    {2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm};
    
    G4double ScintilFast[nEntries] =
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    
    // Add entries into properties table
    G4MaterialPropertiesTable* MPTPolystyrene = new G4MaterialPropertiesTable();
    MPTPolystyrene->AddProperty("RINDEX",PhotonEnergy,RefractiveIndexPS,nEntries);
    MPTPolystyrene->AddProperty("ABSLENGTH",PhotonEnergy,AbsPS,nEntries);
    MPTPolystyrene->AddProperty("FASTCOMPONENT",PhotonEnergy, ScintilFast,nEntries);
    MPTPolystyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
    MPTPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
    MPTPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
    psDet->SetMaterialPropertiesTable(MPTPolystyrene);
    psDet->GetIonisation()->SetBirksConstant(0.009*mm/MeV);
}

/**
 * Sets the optical properties of EJ200
 *
 * Data Sources:
 *
 * EJ200 Data sheet
 */
void Materials::SetOpticalPropertiesEJ200(){
    
    // Index of Reflection (146 nm to 1570 nm)
    const G4int nRINDEX = 2;
    G4double photonEnergyRINDEX[nRINDEX] = {8.5506*eV,0.7973*eV};
    G4double RefractiveIndexGlass[nRINDEX]= {1.58, 1.58};
    
    // Absorbition Length
    const G4int nABS=2;
    G4double photonEnergyABS[nABS] = {3.5*eV,1.75*eV};
    G4double AbsLengthGlass[nABS] = {70*cm, 70*cm};
    
    // Setting sctintillation to be the emission spectra
    const G4int nEM = 6;
    G4double photonEnergyEM[nEM] = {3.01, 2.92, 2.82, 2.70,2.58,2.49};
    G4double emEJ200[nEM]={0,1.0,0.75,0.45,0.18,0.05};
    
    // Creating the materials property table and adding entries into properties table
    G4MaterialPropertiesTable* MPTEJ200 = new G4MaterialPropertiesTable();
    MPTEJ200->AddProperty("RINDEX",photonEnergyRINDEX,RefractiveIndexGlass,nRINDEX);
    MPTEJ200->AddProperty("ABSLENGTH",photonEnergyABS,AbsLengthGlass,nABS);
    // Setting  Scintillation Properties
    MPTEJ200->AddProperty("FASTCOMPONENT",photonEnergyEM,emEJ200,nEM);
    MPTEJ200->AddConstProperty("FASTTIMECONSTANT",2.5*ns);      //
    MPTEJ200->AddConstProperty("SCINTILLATIONYIELD", 10000*MeV);
    MPTEJ200->AddConstProperty("YIELDRATIO", 1.0);
    MPTEJ200->AddConstProperty("RESOLUTIONSCALE", 1.0);
    EJ200->SetMaterialPropertiesTable(MPTEJ200);
}
