#include "PhysicsList.hh"

#include "G4EmDNAPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "G4IonPhysics.hh"
#include "MyIonPhysics.hh"
/**
 * PhysicsList
 *
 * Constructs the physics of the simulation
 */
PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    currentDefaultCut   = 10*nm;

    // SetVerboseLevel(1);

    // Adding Physics List
    RegisterPhysics( new HadronPhysicsQGSP_BERT_HP());
//    RegisterPhysics( new G4IonPhysics());
    RegisterPhysics( new MyIonPhysics(1));
   // RegisterPhysics( new G4HadronElasticPhysicsXS());
   // RegisterPhysics( new G4QStoppingPhysics());
    
    RegisterPhysics( new G4EmStandardPhysics_option4());
    //RegisterPhysics( new G4EmLivermorePhysics());
   // RegisterPhysics( new G4EmDNAPhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList(){
    // Nothing to be done
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts(){

    SetDefaultCutValue(10*nm);
}
