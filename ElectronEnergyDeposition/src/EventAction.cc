#include "EventAction.hh"
#include "RunAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

/**
 * Creates an event hook for each event.
 *
 * The default print is set to every 100,000 events along without drawing 
 * and using the default EventActionMessenger.
 */
EventAction::EventAction(RunAction* run)
:fRunAction(run),fPrintModulo(100000)
{

 
}
/**
 * Deconstructor
 */
EventAction::~EventAction(){
  
}

void EventAction::AddEdep(G4double xPos, G4double Edep){
    G4int index = (int) xPos/fBinWidth;
    fTotalEnergyDeposit[index] += Edep;
}
/**
 * Initilizes the energy deposition of the event and prints the event
 * if the event is the modulus of the fPrintModulo
 *
 * @param evt - the event
 */
void EventAction::BeginOfEventAction(const G4Event*){

    /* Getting the world size */
    G4double worldXHalfLength = 0.5*cm;
    G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box* worldBox = NULL;
    if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    if ( worldBox ) worldXHalfLength = -worldBox->GetXHalfLength();
    else  {
        G4cerr << "World volume of box not found." << G4endl;
        G4cerr << "Setting the detector size to be 1 cm"<<G4endl;
    }
    fWorldSize = worldXHalfLength*2;
    
    /* Creating arrays for the energy deposition */
    G4int fNumBins = fWorldSize/fBinWidth;
    fTotalEnergyDeposit = new double[fNumBins];
    for (G4int i = 0; i<fNumBins; i++) {
        fTotalEnergyDeposit[i] = 0.0;
    }

}
/**
 * Fills the energy deposition at the end of the event. The histogram is filled
 * using the G4AnalysisManagerInstance, as setup in RunAction.
 *
 * @param evt - the event
 */
void EventAction::EndOfEventAction(const G4Event*)
{
    /* Copying over event totals to the run */
    
    for (G4int i = 0; i< fNumBins; i++) {
        fRunAction->AddEdep((G4double) i*fBinWidth,fTotalEnergyDeposit[i]);
    }
    /* Cleaning up the memory */
      delete fTotalEnergyDeposit;
}
