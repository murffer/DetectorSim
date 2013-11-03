#include "Analysis.hh"

#include "G4Event.hh"
#include "G4Run.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4UnitsTable.hh"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
/**
 * Increments the energy deposition for each run
 * @param xPos - the xPosition of the event
 * @param val - the energy deposition
 */

void Analysis::AddEDepRun(G4double xPos, G4double eDep){
    // Incrementing the energy deposition
    if (eDep > 0){
        G4int index = GetBinIndex(xPos);
        // Total in a single bin
        eDepRun[index] += eDep;
        eDepRun2[index] += eDep*eDep;
        // Total of all positions
        eDepRunTotal += eDep;
        eDepRunTotal2 += eDep*eDep;
    }
}
/**
 * Prepares a new run
 *
 * Initilizes the data fields for a new run
 * @param run object
 */
void Analysis::PrepareNewRun(const G4Run* ){
    // Resetting the run energy deposition
    for (G4int i = 0; i < numBins; i++) {
        eDepRun[i] = 0.0;
        eDepRun2[i] = 0.0;
    }
    eDepRunTotal = eDepRunTotal2 = 0.0;
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
 *
 * Normalization of the events are computed
 * @param run object
 */
void Analysis::EndOfRun(const G4Run *aRun){
    G4int NbOfEvents = aRun->GetNumberOfEvent();
    if (NbOfEvents == 0) return;
    G4double dNbOfEvents = double(NbOfEvents);
    for (G4int i = 0; i<numBins; i++) {
        eDepRun[i]/=dNbOfEvents; eDepRun2[i] /= dNbOfEvents;
    }
    eDepRunTotal /= dNbOfEvents; eDepRunTotal2 /= dNbOfEvents;
}

/**
 * Writes the run output to a csv
 * @param name - the name of the particle
 * @param material - the name of the material
 * @param energy - the energy of the primary particle
 */
void Analysis::WriteRun(G4String partName, G4String matName, G4double energy){
    
    G4double eDepRms ,eDepTotal ,eDepTotalErr ,x;
    std::ofstream myfile;
    std::ostringstream filename;
    filename << matName<<"_"<<energy/keV<<"keV.csv";
    myfile.open (filename.str().c_str());
    myfile <<"Position (um),Energy Deposited in Slice (keV),error,Total Energy Deposited (keV),error\n";
    
    eDepRms = eDepTotal = eDepTotalErr = x = 0;
    for (G4int i = 0; i<numBins; i++) {
        x = (posBins[i]-x)/2.0;
        eDepRms = eDepRun2[i] - eDepRun[i]*eDepRun[i];
        if (eDepRms>0.) eDepRms = std::sqrt(eDepRms); else eDepRms = 0.;
        eDepTotal += eDepRun[i];
        eDepTotalErr = sqrt(pow(eDepTotalErr, 2.0)+pow(eDepRms, 2));
        myfile<<x/um<<","<<eDepRun[i]/keV<<","<<eDepRms/keV
        <<","<<eDepTotal<<","<<eDepTotalErr<<"\n";
        x = posBins[i];
    }
    myfile.close();
    
    /* Writing summary data to the screen */
 
    eDepRms = eDepRunTotal2 - eDepRunTotal*eDepRunTotal;
    if (eDepRms>0.) eDepRms = std::sqrt(eDepRms); else eDepRms = 0.;
    G4cout<<"\tRun was "<<G4BestUnit(energy,"Energy")<<" "<<partName<<G4endl;
    G4cout<<"\tTotal Energy Deposited: "<<G4BestUnit(eDepRunTotal,"Energy")
    <<" +/- "<<G4BestUnit(eDepRms,"Energy")
    <<G4endl;
    
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
    // Iterating to the bin
    G4int index = 0;
    while (x > posBins[index]){
        index ++;
    }
    // Error checking
    if (index > numBins || index < 0){
        G4cerr<<"The bin index "<<index<<" execeds the range [0 "<<numBins<<"]"<<G4endl;
        G4cerr<<"\tx position: "<<G4BestUnit(x,"Length")<<" index: "<<index<<G4endl;
    }
    return index;
}