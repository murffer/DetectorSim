#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;
class G4ParticleGun;
class DetectorConstruction;
class Analysis;
class PrimaryGeneratorAction;


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

};
#endif

