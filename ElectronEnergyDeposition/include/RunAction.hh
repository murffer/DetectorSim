#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class G4ParticleGun;
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

   
    /**
     * Increment the energy depositioin
     *
     * @param - the energy deposited in the run
     */
    void AddEdep(G4double val)     { fEdep += val;}
  
  private:
    DetectorConstruction*   fDetector;      /** Pointer to detector geometry */
    PrimaryGeneratorAction* fPrimary;       /** Pointer to primary action    */
    HistoManager*           fHistoManager;  /** Pointer to analysis manager  */
      
    G4double        fEdep;       /** Energy deposition in run    */

};
#endif

