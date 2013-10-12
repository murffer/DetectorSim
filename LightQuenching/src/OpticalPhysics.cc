#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "OpticalPhysics.hh"

OpticalPhysics::OpticalPhysics(G4bool toggle)
    : G4VPhysicsConstructor("Optical")
{
  theProcess                = NULL;
  theScintProcess              = NULL;
  theCerenkovProcess           = NULL;
  theBoundaryProcess           = NULL;
  theAbsorptionProcess         = NULL;
  theRayleighScattering        = NULL;
  theMieHGScatteringProcess    = NULL;

  AbsorptionOn                 = toggle;
}

OpticalPhysics::~OpticalPhysics() { }

//#include "G4ParticleDefinition.hh"
//#include "G4ParticleTable.hh"

#include "G4OpticalPhoton.hh"

void OpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4ProcessManager.hh"

void OpticalPhysics::ConstructProcess()
{
    G4cout << "OpticalPhysics:: Add Optical Physics Processes"
           << G4endl;

  theProcess = new G4Op();

  theScintProcess = new G4Scintillation();
  theScintProcess->SetScintillationYieldFactor(1.);
  theScintProcess->SetTrackSecondariesFirst(true);

  theCerenkovProcess = new G4Cerenkov();
  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  theCerenkovProcess->SetTrackSecondariesFirst(true);

  theAbsorptionProcess      = new G4OpAbsorption();
  theRayleighScattering     = new G4OpRayleigh();
  theMieHGScatteringProcess = new G4OpMieHG();
  theBoundaryProcess        = new G4OpBoundaryProcess();

  G4ProcessManager* pManager =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (!pManager) {
     std::ostringstream o;
     o << "Optical Photon without a Process Manager";
     G4Exception("OpticalPhysics::ConstructProcess()","",
                  FatalException,o.str().c_str());
  }

  if (AbsorptionOn) pManager->AddDiscreteProcess(theAbsorptionProcess);

  //pManager->AddDiscreteProcess(theRayleighScattering);
  //pManager->AddDiscreteProcess(theMieHGScatteringProcess);

  pManager->AddDiscreteProcess(theBoundaryProcess);

  theProcess->UseTimeProfile("delta");
  //theProcess->UseTimeProfile("exponential");

  pManager->AddDiscreteProcess(theProcess);

  theScintProcess->SetScintillationYieldFactor(1.);
  theScintProcess->SetScintillationExcitationRatio(0.0);
  theScintProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process

  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  theScintProcess->AddSaturation(emSaturation);

  theParticleIterator->reset();
  while ( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String particleName = particle->GetParticleName();

    pManager = particle->GetProcessManager();
    if (!pManager) {
       std::ostringstream o;
       o << "Particle " << particleName << "without a Process Manager";
       G4Exception("OpticalPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
    }

    if(theCerenkovProcess->IsApplicable(*particle)){
      pManager->AddProcess(theCerenkovProcess);
      pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
    }
    if(theScintProcess->IsApplicable(*particle)){
      pManager->AddProcess(theScintProcess);
      pManager->SetProcessOrderingToLast(theScintProcess,idxAtRest);
      pManager->SetProcessOrderingToLast(theScintProcess,idxPostStep);
    }

  }
}

void OpticalPhysics::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}
