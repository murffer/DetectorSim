#ifndef EventActionMessenger_h
#define EventActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventActionMessenger: public G4UImessenger
{
  public:
    EventActionMessenger(EventAction*);
   ~EventActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    EventAction*          fEventAction;
    
    G4UIdirectory*        fEventDir;   
    G4UIcmdWithAString*   fDrawCmd;
    G4UIcmdWithAnInteger* fPrintCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
