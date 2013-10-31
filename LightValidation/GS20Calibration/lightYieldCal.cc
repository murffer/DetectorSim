#include "G4RunManager.hh"
#include "G4UImanager.hh"

// My Application
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "PhysicsList.hh"

#include "Randomize.hh"
#include "Analysis.hh"

// MPI session
#ifdef USE_MPI
#include "G4MPImanager.hh"
#include "G4MPIsession.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv){

    // Choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
		// Setup the run Manager
#ifdef USE_MPI
	G4MPImanager* g4MPI = new G4MPImanager(argc, argv);
	G4MPIsession* session = g4MPI-> GetMPIsession();
#endif
    // ----------------- User Application Setting -----------------
    G4RunManager * runManager = new G4RunManager;
		runManager->SetUserInitialization(new DetectorConstruction()); 
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new RunAction());
    runManager->SetUserAction(new EventAction());
    runManager->SetUserAction(new StackingAction());
    runManager->Initialize();

    // Setting up the analysis
    Analysis::GetInstance()->Initilize();


#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

	// --------------------------------------------------------------------
	// Starting the run session
	// --------------------------------------------------------------------
#ifdef USE_MPI
	session-> SessionStart();
#else
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc!=1){   // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else{  // interactive mode : define UI session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute vis.mac"); 
#else
        UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
#endif
    }
#endif // USE MPI

	// --------------------------------------------------------------------
  // Job termination
	// --------------------------------------------------------------------
#ifdef G4VIS_USE
    delete visManager;
#endif
    Analysis::GetInstance()->CleanUp();
#ifdef USE_MPI
		delete g4MPI;
#endif
		delete runManager;
		return 0;
}
