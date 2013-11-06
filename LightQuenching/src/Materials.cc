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
	MPTPolystyrene->AddConstProperty("SCINTILLATIONYIELD",3000/MeV);
	MPTPolystyrene->AddConstProperty("RESOLUTIONSCALE",10);
	MPTPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
	Polystyrene->SetMaterialPropertiesTable(MPTPolystyrene);
	Polystyrene->GetIonisation()->SetBirksConstant(0.2*mm/MeV);
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
