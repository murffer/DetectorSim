#include "PhysicsList.hh"

#include "G4EmDNAPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "G4IonPhysics.hh"

/**
 * PhysicsList
 *
 * Constructs the physics of the simulation
 */
PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    currentDefaultCut   = 10*nm;

    // SetVerboseLevel(1);

    // Adding Physics List
    //RegisterPhysics( new G4EmDNAPhysics());
    RegisterPhysics( new G4EmStandardPhysics());
    RegisterPhysics( new G4EmLivermorePhysics());
    RegisterPhysics( new HadronPhysicsQGSP_BERT_HP());
    RegisterPhysics( new G4IonPhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList(){
    // Nothing to be done
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts(){

    SetDefaultCutValue(10*nm);
}
