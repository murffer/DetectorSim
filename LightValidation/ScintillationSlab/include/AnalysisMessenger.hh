#ifndef AnalysisMessenger_h
#define AnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Analysis;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class AnalysisMessenger: public G4UImessenger
{
  public:
    AnalysisMessenger();
   ~AnalysisMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    G4UIdirectory*             AnalysisDir;
    G4UIcmdWithAString*        ParticleNameCmd;
    G4UIcmdWithADoubleAndUnit* HistEMaxCmd;
};

#endif

