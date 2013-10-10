#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "AnalysisMessenger.hh"

#include "PhysicsList.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv){

    // Choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    // ----------------- User Application Setting -----------------
    G4RunManager * runManager = new G4RunManager;
    runManager->SetUserInitialization(new DetectorConstruction()); 
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new RunAction());
    runManager->SetUserAction(new EventAction());
    runManager->Initialize();

    AnalysisMessenger* aMessenger = new AnalysisMessenger();

#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

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

    // Job termination
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;
    return 0;
}
