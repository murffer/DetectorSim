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
    void AddEdep(G4double, G4double);
  
  private:
    DetectorConstruction*   fDetector;      /** Pointer to detector geometry */
    PrimaryGeneratorAction* fPrimary;       /** Pointer to primary action    */

      
    G4double* fEdep;       /** Energy deposition in run */
    G4double* fEdep2;
    G4double fBinWidth;      /** Bin Sizes */
    G4double fWorldSize;
    G4int fNumBins;     /* Number of width bins */
    

};
#endif

