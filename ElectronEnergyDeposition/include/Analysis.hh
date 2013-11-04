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
    static const G4int numBins = 50;
    static G4double posBins[numBins];

    
    // Helper methods
    G4int GetBinIndex(G4double);
    void SetWorldSize();
    
    // Geoemtry Parameters
    G4double worldSize;
    
    // Accumulation variables
    G4double eDepEvent[numBins];
    G4double eDepRun[numBins];
    G4double eDepRun2[numBins];
    G4double eDepRunTotal;
    G4double eDepRunTotal2;
};


#endif
