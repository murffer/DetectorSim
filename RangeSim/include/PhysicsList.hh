#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class DetectorConstruction;
class PhysicsListMessenger;
class G4VPhysicsConstructor;


class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList(DetectorConstruction*);
   ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
    void AddPhysicsList(const G4String& name);
    
    void AddDecay();
    void AddRadioactiveDecay();

    virtual void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
    void SetCutForAlpha(G4double);
    void SetCutForTriton(G4double);
    void GetRange(G4double);
      
  private:
    G4double fCutForGamma;        /** Default gamma cut    */
    G4double fCutForElectron;     /** Default electron cut */
    G4double fCutForPositron;     /** Default positron cut */
    G4double fCutForAlpha;        /** Defualt alpha cut    */
    G4double fCutForTriton;       /** Default triton cut   */
    G4double fCurrentDefaultCut;  /* Default cut for all particles  */
    
    G4VPhysicsConstructor* fEmPhysicsList;  /* Default physics list       */
    G4String               fEmName;         /* Name of the physics list   */
    
    DetectorConstruction* fDet;             /* Pointer to the detector    */
    PhysicsListMessenger* fMessenger;       /* Physics list messenger     */  
};
#endif
