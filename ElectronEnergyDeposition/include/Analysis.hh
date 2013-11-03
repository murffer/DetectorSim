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
        0.1000*um , 0.6551*um , 4.2919*um , 28.1177*um , 184.2070*um ,
        0.1207*um , 0.7906*um , 5.1795*um , 33.9322*um , 222.2996*um ,
        0.1456*um , 0.9541*um , 6.2506*um , 40.9492*um , 268.2696*um ,
        0.1758*um , 1.1514*um , 7.5431*um , 49.4171*um , 323.7458*um ,
        0.2121*um , 1.3895*um , 9.1030*um , 59.6362*um , 390.6940*um ,
        0.2560*um , 1.6768*um , 10.9854*um , 71.9686*um , 471.4866*um ,
        0.3089*um , 2.0236*um , 13.2571*um , 86.8511*um , 568.9866*um ,
        0.3728*um , 2.4421*um , 15.9986*um , 104.8113*um , 686.6488*um ,
        0.4498*um , 2.9471*um , 19.3070*um , 126.4855*um , 828.6428*um ,
        0.5429*um , 3.5565*um , 23.2995*um , 152.6418*um , 1000.0000*um};
    
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
