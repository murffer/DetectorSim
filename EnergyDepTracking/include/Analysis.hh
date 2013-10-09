#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "TH1F.h"
#include "TH2F.h"
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
        void SetHistEMax(G4double emax);
        void SetBinWidth(G4double binWidth);
        void SetEDepPosAnalysis(G4bool value);
   private:

        // Singleton Analysis
        Analysis();
        static Analysis *singleton;
        G4double GetDetectorThickness();
        G4double GetCalorimeterThickness();
        G4String GetDetectorMaterial();
        G4String incidentParticleName;
        G4double maxHistEnergy;
        G4double posHistBinWidth;
        G4bool EDepPosAnalysis = false;

        // ROOT Output variables
        TFile* outfile;
        TH1F* eDepHist;
        TH2F* posEDepHist;
        TNtuple* posEDepTuple;


        // Accumulation Variables
        G4double eDepEvent;

};
#endif
