#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventActionMessenger;
class RunAction;
class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
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
  
    /**
     * Sets the print modulos
     *
     * @param val - the print modulos
     */
    void SetPrintModulo(G4int val) {fPrintModulo = val;};


  private:
        RunAction*    fRunAction;       /** Pointer to RunAction    */
    G4double fBinWidth = 5*um;      /** Bin Sizes */
    G4double fWorldSize = 1*cm;
    G4double*               fTotalEnergyDeposit; /** Total Energy Depositon  */
    G4int                  fPrintModulo;        /** Print modulos           */
    G4int fNumBins = 0;     /* Number of width bins */

};
#endif
