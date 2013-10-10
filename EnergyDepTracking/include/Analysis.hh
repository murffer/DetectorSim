#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "G4String.hh"

#include "CaloHit.hh"

#include "globals.hh"
class HistoManager;
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
  
   private:

        // Singleton Analysis
        Analysis();
        static Analysis *singleton;

        // Accumulation Variables
        G4double eDepEvent;
        HistoManager* fHistoManager;
};
#endif
