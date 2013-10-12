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
  G4cout<<"Materials - Created all of the materials"<<G4endl;
}

Materials::~Materials(){
  delete EJ200;       
  delete Polystyrene;
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
  G4int ncomponents;
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
  // Polystyrene
  //--------------------------------------------------
  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);
  density = 1.050*g/cm3;
  Polystyrene = nistMan->ConstructNewMaterial("Polystyrene", elements, natoms, density);
  elements.clear();
  natoms.clear();
  
}
/**
 * Sets the Optical Properties for Polystyrene
 *
 */
void Materials::SetOpticalPropertiesPS(){
  //--------------------------------------------------
  //  Polystyrene
  //--------------------------------------------------

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
  Polystyrene->SetMaterialPropertiesTable(MPTPolystyrene);
  Polystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
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
    MPTEJ200->AddConstProperty("SCINTILLATIONYIELD", 10200*MeV);
    MPTEJ200->AddConstProperty("YIELDRATIO", 1.0);
    MPTEJ200->AddConstProperty("RESOLUTIONSCALE", 1.0);
    EJ200->SetMaterialPropertiesTable(MPTEJ200);
}
