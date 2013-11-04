#include "PhysicsList.hh"

#include "G4EmDNAPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "G4IonPhysics.hh"
/**
 * PhysicsList
 *
 * Constructs the physics of the simulation
 *
 * The physics list relies on modular physics. Currently used modules are:
 *  - Hadronic physics (High Precision)
 *  - IonPhysics
 *  - OpticalPhysics
 *  - EM Standard Physics
 */
PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    currentDefaultCut   = 10*nm;

    // SetVerboseLevel(1);

    // Adding Physics List
    RegisterPhysics( new HadronPhysicsQGSP_BERT_HP());
    RegisterPhysics( new G4IonPhysics());
    RegisterPhysics( new G4OpticalPhysics());
    RegisterPhysics( new G4EmStandardPhysics_option4());
   // RegisterPhysics( new G4EmDNAPhysics());
}

/**
 * Deconstructor
 */
PhysicsList::~PhysicsList(){
    // Nothing to be done
}
/**
 * Sets the physic list cuts
 */
void PhysicsList::SetCuts(){
    SetDefaultCutValue(1*um);
}
