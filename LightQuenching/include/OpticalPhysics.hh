#ifndef OpticalPhysics_h
#define OpticalPhysics_h 1

#include "globals.hh"

#include "G4Op.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"

#include "G4OpMieHG.hh"
#include "G4OpRayleigh.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4VPhysicsConstructor.hh"

class OpticalPhysics : public G4VPhysicsConstructor
{
  public:

    OpticalPhysics(G4bool toggle=true);
    virtual ~OpticalPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    G4Op* GetProcess() {return theProcess;}
    G4Cerenkov* GetCerenkovProcess() {return theCerenkovProcess;}
    G4Scintillation* GetScintillationProcess() {return theScintProcess;}
    G4OpAbsorption* GetAbsorptionProcess() {return theAbsorptionProcess;}
    G4OpRayleigh* GetRayleighScatteringProcess() {return theRayleighScattering;}
    G4OpMieHG* GetMieHGScatteringProcess() {return theMieHGScatteringProcess;}
    G4OpBoundaryProcess* GetBoundaryProcess() { return theBoundaryProcess;}

    void SetNbOfPhotonsCerenkov(G4int);

private:

    G4Op*             theProcess;
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScattering;
    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
 
    G4bool AbsorptionOn;

};
#endif
