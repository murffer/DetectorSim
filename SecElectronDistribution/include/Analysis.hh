#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "TH1F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "G4String.hh"

#include "CaloHit.hh"

#include "globals.hh"

class Analysis {

  public:
    // Singleton Class
    static Analysis* GetInstance(){
      if(Analysis::singleton == NULL)
        Analysis::singleton = new Analysis();
      return Analysis::singleton;
    }

    virtual ~Analysis();

    // Accumulation Methods
    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);
    
    void SetIncidentParticleName(G4String pName);
    TH1F* TH1FLog(const char *name,const char* title, int numBins, double xMin,double xMax);
    void MyFill(TH1F* h, double value);

  private:

    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    // Class interanls
    G4double GetDetectorThickness();
    G4String GetDetectorMaterial();
    G4String incidentParticleName;
    bool neutron;

    // ROOT Output variables
    TFile* outfile;
    TNtuple* kinETuple; 
    TH1F* kinEHist;
    TH1F* numSecHist;
    TH1F* kEAlphaHist;  // Only for neturons
    TH1F* kETritonHist;
    TH1F* kEAlphaHistLog;
    TH1F* kETritonHistLog;
    TH1F* nSAlphaHist;
    TH1F* nSTritonHist;
    TNtuple* aKinETuple; 
    TNtuple* tKinETuple; 

    G4int numSec;
    G4int numSecAlpha;
    G4int numSecTriton;
};
#endif
