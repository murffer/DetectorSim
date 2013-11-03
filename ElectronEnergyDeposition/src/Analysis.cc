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