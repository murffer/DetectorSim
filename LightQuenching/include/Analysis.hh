#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "TH1F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "G4String.hh"

class PMTHit;
class AbsorberHit;
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
    void SetNumOpticalPhotonsGenerated(G4int numPhotons);
  
  private:

    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    // Class interanls
    G4String GetDetectorMaterial();   /* Gets the detector material */
    G4String incidentParticleName;    /* Gets the incident particle */
    bool neutron;                     /* True for a neutron incident*/

    // ROOT Output variables
    TFile* outfile;             /* ROOT output file handle          */
    TH1F* eDepHist;             /* Event Energy Deposition Histogram*/
    TH1F* opAbsHist;            /* Event Optical Photon (op) Absorber (created) histogram   */
    TH1F* opPMTHist;            /* Event Optical Photon (op) PMT (created) histogram        */
    G4double maxHistEnergy;     /* Maximum Energy that can be deposited in an event         */
    G4int maxNumPhotons;        /* Maxium number of photons that can be created in an event */
    
    // Data Collections Varaibles
    G4double eDepEvent;         /* Energy deposition in the absoerber */
    G4int nOPAbsEvent;          /* Number optical photons created     */
    G4int nOPPMTEvent;          /* Number optical photons detected    */
};
#endif
