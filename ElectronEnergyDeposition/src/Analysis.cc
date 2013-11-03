#include "Analysis.hh"

#include "G4Event.hh"
#include "G4Run.hh"


#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

Analysis* Analysis::singleton = 0;
/**
 * Analysis
 *
 * Creates an Analysis object, setting the world size
 */
Analysis::Analysis(){
    SetWorldSize();
}

/**
 * Adds the energy deposition of a step to the event
 * @param xPos - the x position of the event
 * @param eDep - the energy deposition of the event
 */
void Analysis::AddEDepEvent(G4double xPos, G4double eDep){
    if (eDep > 0){
        G4int index = GetBinIndex(xPos+worldSize*0.5);
        eDepEvent[index] += eDep;
    }
}
void Analysis::AddEDepRun(G4double xPos, G4double eDep){
    
}
/**
 * Prepares a new run
 *
 * Initilizes the data fields for a new run
 * @param run object
 */
void Analysis::PrepareNewRun(const G4Run*){
    // Resetting the run energy deposition
    for (G4int i = 0; i < numBins; i++) {
        eDepRun[i] = 0.0;
        eDepRun2[i] = 0.0;
    }
}
/**
 * Prepares a new event
 *
 * Initilizes the data fields for a new event
 * @param event object
 */
void Analysis::PrepareNewEvent(const G4Event*){
    // Resetting the event energy deposition
    for (G4int i = 0; i < numBins; i++) {
        eDepEvent[i] = 0;
    }
}

/**
 * Called at the end of an evnet
 * @param event object
 */
void Analysis::EndOfEvent(const G4Event*){
    /* Copying over event totals to the run */
    for (G4int i = 0; i< numBins; i++) {
        AddEDepRun(posBins[i], eDepEvent[i]);
    }
}

/**
 * Called at the end of an event
 * @param run object
 */
void Analysis::EndOfRun(const G4Run *aRun){
    
}
/**
 * Sets the world size
 *
 * Sets the world size based on the name of the world.
 * The goemetry is acessed through the physical volume store.
 */
void Analysis::SetWorldSize(){
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
    worldSize = worldXHalfLength*2;
}
/**
 * Gets the bin index of a float
 * @param the float value
 * @return the corresponding index
 */
G4int Analysis::GetBinIndex(G4double x){
    // Iterating down to the bin
    G4int index = numBins-1;
    while (x <= posBins[index] && index >= 0)
        index --;
    
    // Error checking
    if (index > numBins || index < 0){
        G4cerr<<"The bin index "<<index<<" execeds the range [0 "<<numBins<<"]"<<G4endl;
        G4cerr<<" x position: "<<x/um<<" index: "<<index<<G4endl;
    }
    return index;
}