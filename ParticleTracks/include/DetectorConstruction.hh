#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class Materials;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

  public:

    virtual G4VPhysicalVolume* Construct();
    void SetMaterial (G4String);            
    void UpdateGeometry();

  public:

    /**
     * Returns the physical world volume
     */
    const G4VPhysicalVolume* GetWorld()      {return fPBox;};           
    /**
     * Returns the detector size
     */
    G4double           GetSize()       {return fBoxSize;};      
    /**
     * Returns the detector material
     */
    G4Material*        GetMaterial()   {return fMaterial;};

    void               PrintParameters();

  private:

    G4VPhysicalVolume*    fPBox;          /** Physical Volume  */
    G4LogicalVolume*      fLBox;          /** Logical Volume   */

    G4double              fBoxSize;       /** Length of side of a box  */
    G4Material*           fMaterial;      /** Material of the detector */

    G4double              fMaxStep;       /** Maximum Step Size  */
    Materials* materials;                 

  private:

    G4VPhysicalVolume* ConstructVolumes();     
};
#endif
