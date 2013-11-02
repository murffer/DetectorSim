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
:fRunAction(run)
{
    fBinWidth = 5*um;      /** Bin Sizes */
    fWorldSize = 1*cm;
    fNumBins = 0;     /* Number of width bins */
    
}
/**
 * Deconstructor
 */
EventAction::~EventAction(){
    
}

void EventAction::AddEdep(G4double xPos, G4double Edep){
    G4int index = (int) (xPos+fWorldSize)/fBinWidth;
    fTotalEnergyDeposit[index] += Edep;
 //   G4cout<<"Added energy "<<Edep/keV<<" to bin "<<index<<" at xPos"<<G4endl;
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
    if ( worldBox ) worldXHalfLength = worldBox->GetXHalfLength();
    else  {
        G4cerr << "World volume of box not found." << G4endl;
        G4cerr << "Setting the detector size to be 1 cm"<<G4endl;
    }
    fWorldSize = worldXHalfLength*2;
    
    /* Creating arrays for the energy deposition */
    fNumBins = fWorldSize/fBinWidth;
    fTotalEnergyDeposit = new G4double[fNumBins];
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
   //     if (fTotalEnergyDeposit[i] > 0)
  //          G4cout<<fTotalEnergyDeposit[i]/keV<<G4endl;
        fRunAction->AddEdep(G4double(i*fBinWidth),fTotalEnergyDeposit[i]);
    }
    /* Cleaning up the memory */
    delete fTotalEnergyDeposit;
}
