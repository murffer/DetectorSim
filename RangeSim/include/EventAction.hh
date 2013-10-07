//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
   ~EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void AddEdep(G4double Edep)    {fTotalEnergyDeposit += Edep;};      
    G4double GetEnergyDeposit()    {return fTotalEnergyDeposit;};    
    void SetDrawFlag(G4String val) {fDrawFlag = val;};
    void SetPrintModulo(G4int val) {fPrintModulo = val;};
            
    
  private:
    G4double               fTotalEnergyDeposit;   
    G4String               fDrawFlag;
    G4int                  fPrintModulo;
    
    EventActionMessenger*  fEventMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
