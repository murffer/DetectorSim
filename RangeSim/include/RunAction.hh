#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class G4GeneralParticleSource;
class DetectorConstruction;
class PrimaryGeneratorAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction();

  public:
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    
    void CountTraks0(G4int nt) { fNbOfTraks0 += nt;}
    void CountTraks1(G4int nt) { fNbOfTraks1 += nt;}
    void CountSteps0(G4int ns) { fNbOfSteps0 += ns;}
    void CountSteps1(G4int ns) { fNbOfSteps1 += ns;}
    void CountProcesses(G4String procName) { fProcCounter[procName]++;};
    
    void AddEdep(G4double val)     { fEdep += val;}
    void AddTrueRange (G4double l) { fTrueRange += l; fTrueRange2 += l*l;};
    void AddProjRange (G4double x) { fProjRange += x; fProjRange2 += x*x;};
    void AddTransvDev (G4double y) { fTransvDev += y; fTransvDev2 += y*y;};    
                                  
  private:
    DetectorConstruction*   fDetector;
    PrimaryGeneratorAction* fPrimary;
    HistoManager*           fHistoManager;
      
    G4int           fNbOfTraks0, fNbOfTraks1;
    G4int           fNbOfSteps0, fNbOfSteps1;
    G4double        fEdep;
    G4double        fTrueRange, fTrueRange2;             
    G4double        fProjRange, fProjRange2;
    G4double        fTransvDev, fTransvDev2;
    std::map<G4String,G4int>    fProcCounter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

