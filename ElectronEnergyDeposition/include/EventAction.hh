#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventActionMessenger;
class Analysis;
class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction();
    
public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    /**
     * Increment the energy deposition of the event
     *
     * @param Edep - the energy deposition of the event
     */
    void AddEdep(G4double,G4double);
    
    
private:
    
};
#endif
