#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"


/**
 * Analysis Object
 *
 */
class G4Event;
class G4Run;
class Analysis {
    
public:
    ~Analysis();
    static Analysis* GetInstance(){
        if(Analysis::singleton == NULL)
            Analysis::singleton = new Analysis();
        return Analysis::singleton;
    }
    
    // Accumulation Methods
    void AddEDepEvent(G4double xPos, G4double eDep);
    void AddEDepRun(G4double xPos, G4double eDep);
    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);
    void WriteRun(G4String partName,G4String matName,G4double e);
    
private:
    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    // Fixed Bin Positions
    const G4int numBins = 50;
    const G4double posBins[50] = {
        0.1000*cm , 0.6551*cm , 4.2919*cm , 28.1177*cm , 184.2070*cm ,
        0.1207*cm , 0.7906*cm , 5.1795*cm , 33.9322*cm , 222.2996*cm ,
        0.1456*cm , 0.9541*cm , 6.2506*cm , 40.9492*cm , 268.2696*cm ,
        0.1758*cm , 1.1514*cm , 7.5431*cm , 49.4171*cm , 323.7458*cm ,
        0.2121*cm , 1.3895*cm , 9.1030*cm , 59.6362*cm , 390.6940*cm ,
        0.2560*cm , 1.6768*cm , 10.9854*cm , 71.9686*cm , 471.4866*cm ,
        0.3089*cm , 2.0236*cm , 13.2571*cm , 86.8511*cm , 568.9866*cm ,
        0.3728*cm , 2.4421*cm , 15.9986*cm , 104.8113*cm , 686.6488*cm ,
        0.4498*cm , 2.9471*cm , 19.3070*cm , 126.4855*cm , 828.6428*cm ,
        0.5429*cm , 3.5565*cm , 23.2995*cm , 152.6418*cm , 1000.0000*cm};
    
    // Helper methods
    G4int GetBinIndex(G4double);
    void SetWorldSize();
    
    // Geoemtry Parameters
    G4double worldSize;
    
    // Accumulation variables
    G4double eDepEvent[50];
    G4double eDepRun[50];
    G4double eDepRun2[50];
    G4double eDepRunTotal;
    G4double eDepRunTotal2;
};

#endif
