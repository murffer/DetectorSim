#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventActionMessenger;
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
    void AddEdep(G4double Edep)    {fTotalEnergyDeposit += Edep;};      
    /**
     * @return the energy deposition of the event
     */
    G4double GetEnergyDeposit()    {return fTotalEnergyDeposit;};    
    /**
     * Sets the draw flag
     */
    void SetDrawFlag(G4String val) {fDrawFlag = val;};
    /**
     * Sets the print modulos
     *
     * @param val - the print modulos
     */
    void SetPrintModulo(G4int val) {fPrintModulo = val;};


  private:
    G4double               fTotalEnergyDeposit; /** Total Energy Depositon  */
    G4String               fDrawFlag;           /** Flag to draw the energy */
    G4int                  fPrintModulo;        /** Print modulos           */

};
#endif
