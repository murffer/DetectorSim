#include "G4RunManager.hh"
#include "G4UImanager.hh"

// My Applications
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "Analysis.hh"
#include "PhysicsList.hh"

// MPI session
#include "G4MPImanager.hh"
#include "G4MPIsession.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main(int argc,char** argv){

	// Choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

	// --------------------------------------------------------------------
	// MPI session
	// --------------------------------------------------------------------
	// At first, G4MPImanager/G4MPIsession should be created.
	G4MPImanager* g4MPI = new G4MPImanager(argc, argv);

	// MPI session (G4MPIsession) instead of G4UIterminal
	// Terminal availability depends on your MPI implementation.
	G4MPIsession* session = g4MPI-> GetMPIsession();

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
	// ready for go
	// MPIsession treats both interactive and batch modes.
	// Just start your session as below.
	// --------------------------------------------------------------------
	session-> SessionStart();

	// Job termination
#ifdef G4VIS_USE
	delete visManager;
#endif
	Analysis::GetInstance()->CleanUp();
	delete g4MPI;
	delete runManager;
	return 0;
}
