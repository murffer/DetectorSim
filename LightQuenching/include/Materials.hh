#ifndef Materials_h
#define Materials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

/**
 * Constructor for the Optical Materials
 *
 * The optical materials are implemented as a static class
 */
class Materials{

  public:
    ~Materials();
    static Materials* GetInstance();
    G4Material* GetMaterial(const G4String);

  private:
    // Methods
    Materials();
    void CreateMaterials();
    void SetOpticalPropertiesPS();
    void SetOpticalPropertiesEJ200();
    
    // Fields
    static Materials* instance;   /* Materials Instance        */
    G4NistManager* nistMan;  /* NIST Material Manager          */
    G4Material* EJ200;        /* EJ200 Detector                  */
    G4Material* Polystyrene;         /* PMT Window Glass (Boroscilate) */
};

#endif
