/**
 * rangeSim.cc
 *
 * Simulation of alpha, tritons, electrons and photons
 *
 * Matthew J. Urffer (matthew.urffer@gmail.com)
 *
 * This code is based on TestEm1 from the GEANT4 examples.
 */ 

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

/**
 * Setup and run the GEANT4 Simulation
 */
int main(int argc,char** argv) {
 
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  DetectorConstruction* det;
  PrimaryGeneratorAction* prim;
  runManager->SetUserInitialization(det = new DetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList(det));
  runManager->SetUserAction(prim = new PrimaryGeneratorAction());
      
  // set user action classes
  RunAction*   run;
  EventAction* event;
  
  runManager->SetUserAction(run = new RunAction(det,prim)); 
  runManager->SetUserAction(event = new EventAction());
  runManager->SetUserAction(new SteppingAction(event));
     
  // get the pointer to the User Interface manager 
    G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }
    
  else           //define visualization and UI terminal for interactive mode
    {
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
      ui->SessionStart();
      delete ui;
#endif
    }

  // job termination
  delete runManager;
  return 0;
}
