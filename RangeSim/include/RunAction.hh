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
    /**
     * Increments number of neutral tracks
     *
     * @param nt - number of tracks
     */
    void CountTraks0(G4int nt) { fNbOfTraks0 += nt;}
    /**
     * Increments number of charged tracks
     *
     * @param nt - number of tracks
     */
    void CountTraks1(G4int nt) { fNbOfTraks1 += nt;}
    /**
     * Increments number of neutral secondary steps
     *
     * @param ns - number of steps
     */
    void CountSteps0(G4int ns) { fNbOfSteps0 += ns;}
    /**
     * Increments number of charged secondary steps
     *
     * @param ns - number of steps
     */
    void CountSteps1(G4int ns) { fNbOfSteps1 += ns;}
    /**
     * Increments the process
     *
     * @param procName - the name of the process
     */
    void CountProcesses(G4String procName) { fProcCounter[procName]++;};
   
    /**
     * Increment the energy depositioin
     *
     * @param - the energy deposited in the run
     */
    void AddEdep(G4double val)     { fEdep += val;}
    /**
     * Incrment the true range of the particle
     *
     * @param l - the incrmental true range
     */
    void AddTrueRange (G4double l) { fTrueRange += l; fTrueRange2 += l*l;};
    /**
     * Increment the projected range
     *
     * @param x - the project range
     */
    void AddProjRange (G4double x) { fProjRange += x; fProjRange2 += x*x;};
    /**
     * Increment the transverse range
     *
     * @param y - the transverse range
     */
    void AddTransvDev (G4double y) { fTransvDev += y; fTransvDev2 += y*y;};    
                                  
  private:
    DetectorConstruction*   fDetector;      /** Pointer to detector geometry */
    PrimaryGeneratorAction* fPrimary;       /** Pointer to primary action    */
    HistoManager*           fHistoManager;  /** Pointer to analysis manager  */
      
    G4int           fNbOfTraks0; /** Average number of neutral tracks  */ 
    G4int           fNbOfTraks1; /** Average number of charged tracks  */ 
    G4int           fNbOfSteps0; /** Average number of netural steps   */
    G4int           fNbOfSteps1; /** Average number of charged steps   */
    G4double        fEdep;       /** Energy deposition in run    */
    G4double        fTrueRange;  /** True range       */           
    G4double        fProjRange;  /** Projected range  */
    G4double        fTransvDev;  /** Transverse range */
    std::map<G4String,G4int>    fProcCounter; /** Number of processes */
    G4double        fTrueRange2;   /** Square of true range */
    G4double        fProjRange2;  /** Square of projected range */
    G4double        fTransvDev2;  /** Square of transverse range */
};
#endif

