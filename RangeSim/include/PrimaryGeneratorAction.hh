#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void SetDefaultKinematic(G4int);
    void SetRndmBeam(G4double val)  {fRndmBeam = val;}   
    virtual void GeneratePrimaries(G4Event*);
    
    G4ParticleGun* GetParticleGun() {return fParticleGun;}

  private:
    G4ParticleGun*             fParticleGun;
    DetectorConstruction*      fDetector;
    G4double                   fRndmBeam;       
    PrimaryGeneratorMessenger* fGunMessenger;     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


