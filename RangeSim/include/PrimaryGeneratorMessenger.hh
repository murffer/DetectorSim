#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
   ~PrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction* Action;
    
    G4UIdirectory*             fGunDir; 
    G4UIcmdWithAnInteger*      fDefaultCmd;
    G4UIcmdWithADouble*        fRndmCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

