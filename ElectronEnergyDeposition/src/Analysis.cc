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
    
    G4double eDepRms ,eDepTotal ,eDepTotalErr;
    std::ofstream myfile;
    std::ostringstream filename;
    filename << matName<<"_"<<energy/keV<<"keV.csv";
    myfile.open (filename.str().c_str());
    myfile <<"Position (um),Energy Deposited in Slice (keV),error,Total Energy Deposited (keV),error\n";
    
    eDepRms = eDepTotal = eDepTotalErr = 0;
    for (G4int i = 0; i<numBins; i++) {
    
        eDepRms = eDepRun2[i] - eDepRun[i]*eDepRun[i];
        if (eDepRms>0.) eDepRms = std::sqrt(eDepRms); else eDepRms = 0.;
        eDepTotal += eDepRun[i];
        eDepTotalErr = sqrt(pow(eDepTotalErr, 2.0)+pow(eDepRms, 2));
        myfile<<posBins[i]/um<<","<<eDepRun[i]/keV<<","<<eDepRms/keV
        <<","<<eDepTotal<<","<<eDepTotalErr<<"\n";
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

G4double Analysis::posBins[] = {
    0.1000*um , 0.1097*um , 0.1205*um , 0.1322*um , 0.1451*um ,
    0.1592*um , 0.1748*um , 0.1918*um , 0.2105*um , 0.2310*um ,
    0.2535*um , 0.2783*um , 0.3054*um , 0.3352*um , 0.3678*um ,
    0.4037*um , 0.4431*um , 0.4863*um , 0.5337*um , 0.5857*um ,
    0.6428*um , 0.7055*um , 0.7743*um , 0.8498*um , 0.9326*um ,
    1.0235*um , 1.1233*um , 1.2328*um , 1.3530*um , 1.4850*um ,
    1.6298*um , 1.7886*um , 1.9630*um , 2.1544*um , 2.3645*um ,
    2.5950*um , 2.8480*um , 3.1257*um , 3.4305*um , 3.7649*um ,
    4.1320*um , 4.5349*um , 4.9770*um , 5.4623*um , 5.9948*um ,
    6.5793*um , 7.2208*um , 7.9248*um , 8.6975*um , 9.5455*um ,
    10.4762*um , 11.4976*um , 12.6186*um , 13.8489*um , 15.1991*um ,
    16.6810*um , 18.3074*um , 20.0923*um , 22.0513*um , 24.2013*um ,
    26.5609*um , 29.1505*um , 31.9927*um , 35.1119*um , 38.5353*um ,
    42.2924*um , 46.4159*um , 50.9414*um , 55.9081*um , 61.3591*um ,
    67.3415*um , 73.9072*um , 81.1131*um , 89.0215*um , 97.7010*um ,
    107.2267*um , 117.6812*um , 129.1550*um , 141.7474*um , 155.5676*um ,
    170.7353*um , 187.3817*um , 205.6512*um , 225.7020*um , 247.7076*um ,
    271.8588*um , 298.3647*um , 327.4549*um , 359.3814*um , 394.4206*um ,
    432.8761*um , 475.0810*um , 521.4008*um , 572.2368*um , 628.0291*um ,
    689.2612*um , 756.4633*um , 830.2176*um , 911.1628*um , 1000.0000*um};
