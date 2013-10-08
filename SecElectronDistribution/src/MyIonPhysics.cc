#include "MyIonPhysics.hh"

#include "G4DeuteronInelasticProcess.hh"
#include "G4LEDeuteronInelastic.hh"

#include "G4TritonInelasticProcess.hh"
#include "G4LETritonInelastic.hh"

#include "G4AlphaInelasticProcess.hh"
#include "G4LEAlphaInelastic.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4alphaIonisation.hh"
// Nuclei
#include "G4IonConstructor.hh"

MyIonPhysics::MyIonPhysics(G4int verbose):
  G4VPhysicsConstructor(), wasActivated(false)
{ /* nothing to be done here */}

MyIonPhysics::~MyIonPhysics(){
  if(wasActivated) {

    delete fDeuteronProcess;
    delete fDeuteronModel;
    delete fTritonProcess;
    delete fTritonModel;
    delete fAlphaProcess;
    delete fAlphaModel;

   }
 }

void MyIonPhysics::ConstructProcess()
{
  G4ProcessManager * pManager = 0;

 // Deuteron
  pManager = G4Deuteron::Deuteron()->GetProcessManager();
  // add process
  fDeuteronModel = new G4LEDeuteronInelastic();
  fDeuteronProcess = new G4DeuteronInelasticProcess();
  fDeuteronProcess->RegisterMe(fDeuteronModel);
  pManager->AddDiscreteProcess(fDeuteronProcess);

  // Triton
  pManager = G4Triton::Triton()->GetProcessManager();
  // add process
  fTritonModel = new G4LETritonInelastic();
  fTritonProcess = new G4TritonInelasticProcess();
  fTritonProcess->RegisterMe(fTritonModel);
  pManager->AddDiscreteProcess(fTritonProcess);

  // Alpha
  pManager = G4Alpha::Alpha()->GetProcessManager();
  // add process
  fAlphaModel = new G4LEAlphaInelastic();
  fAlphaProcess = new G4AlphaInelasticProcess();
  fAlphaProcess->RegisterMe(fAlphaModel);
  //pManager->AddProcess(new G4alphaIonisation());
  pManager->AddDiscreteProcess(fAlphaProcess);
  wasActivated = true;
}

 void MyIonPhysics::ConstructParticle()
 {
   //  Construct light ions
   G4IonConstructor pConstructor;
   pConstructor.ConstructParticle();  
 }

