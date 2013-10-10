#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(){
  fParticleGun = new G4GeneralParticleSource();

}


PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

