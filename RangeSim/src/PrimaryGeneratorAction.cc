//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(
                                               DetectorConstruction* DC)
:fDetector(DC)                                               
{
  fParticleGun  = new G4ParticleGun(1);
  SetDefaultKinematic(1);
  fRndmBeam = 0.;
    
  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetDefaultKinematic(G4int front)
{
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleEnergy(100*MeV);
  G4double position = 0.*cm;
  if (front) position = -0.5*(fDetector->GetSize());
  fParticleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  //
  //randomize the beam, if requested.
  if (fRndmBeam > 0.) 
    {
      G4ThreeVector oldPosition = fParticleGun->GetParticlePosition();    
      G4double rbeam = 0.5*(fDetector->GetSize())*fRndmBeam;
      G4double x0 = oldPosition.x();
      G4double y0 = oldPosition.y() + (2*G4UniformRand()-1.)*rbeam;
      G4double z0 = oldPosition.z() + (2*G4UniformRand()-1.)*rbeam;
      fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
      fParticleGun->GeneratePrimaryVertex(anEvent);
      fParticleGun->SetParticlePosition(oldPosition);      
    }
  else  fParticleGun->GeneratePrimaryVertex(anEvent); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

