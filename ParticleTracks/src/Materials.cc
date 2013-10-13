#include "Materials.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

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
  G4cout<<"Materials - Created all of the materials"<<G4endl;
}

Materials::~Materials(){
  delete GS20;        /* GS20 Detector  */
  delete Air;         /* Air            */
  delete PMMA;        /* PMMA           */
  delete EJ426;       /* EJ426          */ 
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

    // Material Definations
    
    //-----------------------------------------------------------------------
    // GS20
    // GS20 is (by mass fraction):
    //  56% SiO2, 4% MgO,  18% Al2O3 18% Li2O, and 4% Ce2O3
    //-----------------------------------------------------------------------
    G4Isotope* Li6 = new G4Isotope("Li6",3,6,6.015*g/mole);
    G4Isotope* Li7 = new G4Isotope("Li7",3,7,7.015*g/mole);
    G4Element* enrichLi  = new G4Element("enriched Lithium","Li",2);
    enrichLi->AddIsotope(Li6,0.95*perCent);
    enrichLi->AddIsotope(Li7,0.05*perCent);

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
    G4cout<<EJ426;

    
    //----------------------------------------------------------------------
    // AIR
    //----------------------------------------------------------------------
    Air = nistMan->FindOrBuildMaterial("G4_AIR");
    
     
    //----------------------------------------------------------------------
    // Getting other materials
    //----------------------------------------------------------------------
    PMMA =  nistMan->FindOrBuildMaterial("G4_PLEXIGLASS");
    nistMan->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
    nistMan->FindOrBuildMaterial("G4_POLYSTYRENE");
    nistMan->FindOrBuildMaterial("G4_Pb");
    nistMan->FindOrBuildMaterial("G4_POLYETHYLENE");
    nistMan->FindOrBuildMaterial("G4_Galactic");
}
