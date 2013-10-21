#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4RotationMatrix.hh"

class G4Box;
class G4Tubs;
class G4EllipticalTube;

class G4LogicalVolume;
class G4VPhysicalVolume;

class Materials;
class G4Material;

class DetectorMessenger;

class PhotonDetSD;

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
    G4VPhysicalVolume* ConstructDetector();

    void UpdateGeometry();
 
    // Set Material Commands for World and WLSfiber
    void SetWorldMaterial         (G4String);
    void SetFiberMaterial      (G4String);
    void SetCoupleMaterial        (G4String);

    void SetPhotonDetGeometry     (G4String);
    void SetNumberOfCladding      (G4int);        // Maximum 2 claddings

    void SetLength             (G4double);     // Total length of  fiber
    void SetClad1Thickness        (G4double);
    void SetClad2Thickness        (G4double);
    void SetPhotonDetHalfLength   (G4double);


    // Set the Roughness in between each layer
    void SetSurfaceRoughness      (G4double);
    // Set the reflectivity of the mirror
    void SetMirrorReflectivity    (G4double);
    // Set the polish of the mirror
    void SetMirrorPolish          (G4double);
    // Set the reflectivity of the mirror
    void SetPhotonDetReflectivity (G4double);
    // Set the polish of the mirror
    void SetPhotonDetPolish       (G4double);



    G4double GetLength();
    G4double GetEnd();
    G4double GetFiberRMax();
    G4double GetSurfaceRoughness();

    G4double GetClad1Thickness();
    G4double GetClad2Thickness();
 
    // stringToRotationMatrix() converts a string "X90,Y45" into a
    // G4RotationMatrix.
    // This is an active rotation, in that the object is first rotated
    // around the parent's X axis by 90 degrees, then the object is
    // further rotated around the parent's Y axis by 45 degrees.
    // The return value points to a G4RotationMatrix on the heap, so
    // it is persistent. Angles are in degrees, can have decimals,
    // and can be negative. Axes are X, Y, Z.

    static G4RotationMatrix stringToRotationMatrix(G4String rotation);

    G4Material* FindMaterial(G4String);

  private:

    Materials* materials;

    G4LogicalVolume* logicHole;
    G4LogicalVolume* logicWorld;

    G4VPhysicalVolume* physiWorld;
    G4VPhysicalVolume* physiHole;
 
    G4double           worldSizeX;
    G4double           worldSizeY;
    G4double           worldSizeZ;

    G4double           scintX;
    G4double           scintY;
    G4double           scintZ;

    G4double           clad1Thickness;
    G4double           clad2Thickness;

    G4double           wlsThickenss;
    G4double           clrfiberHalfL;
    G4double           clrfiberZ;

    G4double           coupleRX;
    G4double           coupleRY;
    G4double           coupleZ;
 
 
    G4String           mppcShape;
    G4double           mppcHalfL;
    G4double           mppcZ;
    G4double           mppcDist;
    G4double           mppcTheta;

    G4double wlsfiberOrigin;
    G4double coupleOrigin;
    G4double mirrorOrigin;
    G4double mppcOriginX;
    G4double mppcOriginZ;
 
    G4int numOfCladLayers;

    G4double mirrorPolish;
    G4double mirrorReflectivity;
    G4double mppcPolish;
    G4double mppcReflectivity;
    G4double extrusionPolish;
    G4double extrusionReflectivity;
    G4double surfaceRoughness;

    static PhotonDetSD* mppcSD;

  private:

     void ConstructFiber();

     void UpdateGeometryParameters();

     DetectorMessenger* detectorMessenger;

};

#endif
