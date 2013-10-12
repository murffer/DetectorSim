#include "PhysicsList.hh"

#include "ExtraPhysics.hh"
#include "OpticalPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysListFactory.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "StepMax.hh"

#include "G4ProcessTable.hh"

#include "G4PionDecayMakeSpin.hh"
#include "G4DecayWithSpin.hh"

#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"

#include "G4RadioactiveDecayPhysics.hh"

#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList(G4String physName) : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();

    defaultCutValue  = 1.*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;

    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
    if (factory.IsReferencePhysList(physName)) {
       phys = factory.GetReferencePhysList(physName);
       if(!phys)G4Exception("PhysicsList::PhysicsList","InvalidSetup",
                            FatalException,"PhysicsList does not exist");
    }

    for (G4int i = 0; ; ++i) {
       G4VPhysicsConstructor* elem =
                  const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
       if (elem == NULL) break;
       G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
       RegisterPhysics(elem);
    }

    AbsorptionOn = true;

    physicsVector->push_back(new ExtraPhysics());
    physicsVector->push_back(opticalPhysics = new OpticalPhysics(AbsorptionOn));

    physicsVector->push_back(new G4RadioactiveDecayPhysics());

    stepMaxProcess = new StepMax();
}

PhysicsList::~PhysicsList()
{
    delete fMessenger;

    delete stepMaxProcess;
}

void PhysicsList::ClearPhysics()
{
    for (G4PhysConstVector::iterator p  = physicsVector->begin();
                                     p != physicsVector->end(); ++p) {
        delete (*p);
    }
    physicsVector->clear();
}

void PhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();

    G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
    MuonPlusDecayTable -> Insert(new
                           G4MuonDecayChannelWithSpin("mu+",0.986));
    MuonPlusDecayTable -> Insert(new
                           G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
    G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);

    G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
    MuonMinusDecayTable -> Insert(new
                            G4MuonDecayChannelWithSpin("mu-",0.986));
    MuonMinusDecayTable -> Insert(new
                            G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
    G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);

}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();

    SetVerbose(0);

    G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();

    G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();

    G4VProcess* decay;
    decay = processTable->FindProcess("Decay",G4MuonPlus::MuonPlus());

    G4ProcessManager* fManager;
    fManager = G4MuonPlus::MuonPlus()->GetProcessManager();

    if (fManager) {
      if (decay) fManager->RemoveProcess(decay);
      fManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      fManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      fManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4MuonMinus::MuonMinus());

    fManager = G4MuonMinus::MuonMinus()->GetProcessManager();

    if (fManager) {
      if (decay) fManager->RemoveProcess(decay);
      fManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      fManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      fManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    G4PionDecayMakeSpin* poldecay = new G4PionDecayMakeSpin();

    decay = processTable->FindProcess("Decay",G4PionPlus::PionPlus());

    fManager = G4PionPlus::PionPlus()->GetProcessManager();

    if (fManager) {
      if (decay) fManager->RemoveProcess(decay);
      fManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      fManager ->SetProcessOrdering(poldecay, idxPostStep);
      fManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4PionMinus::PionMinus());

    fManager = G4PionMinus::PionMinus()->GetProcessManager();

    if (fManager) {
      if (decay) fManager->RemoveProcess(decay);
      fManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      fManager ->SetProcessOrdering(poldecay, idxPostStep);
      fManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    AddStepMax();

}

void PhysicsList::RemoveFromPhysicsList(const G4String& name)
{
    G4bool success = false;
    for (G4PhysConstVector::iterator p  = physicsVector->begin();
                                     p != physicsVector->end(); ++p) {
        G4VPhysicsConstructor* e = (*p);
        if (e->GetPhysicsName() == name) {
           physicsVector->erase(p);
           success = true;
           break;
        }
    }
    if (!success) {
       G4ExceptionDescription message;
       message << "PhysicsList::RemoveFromEMPhysicsList "<< name << "not found";
       G4Exception("example PhysicsList::RemoveFromPhysicsList()",
       "ExamPhysicsList01",FatalException,message);
    }
}

void PhysicsList::SetAbsorption(G4bool toggle)
{
       AbsorptionOn = toggle;
       RemoveFromPhysicsList("Optical");
       physicsVector->push_back(opticalPhysics = new OpticalPhysics(toggle));
       opticalPhysics->ConstructProcess();
}

void PhysicsList::SetCuts()
{
    if (verboseLevel >0) {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length")
               << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");

    if (verboseLevel>0) DumpCutValuesTable();
}

void PhysicsList::SetCutForGamma(G4double cut)
{
    fCutForGamma = cut;
    SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

void PhysicsList::SetCutForElectron(G4double cut)
{
    fCutForElectron = cut;
    SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

void PhysicsList::SetCutForPositron(G4double cut)
{
    fCutForPositron = cut;
    SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

void PhysicsList::SetStepMax(G4double step)
{
  MaxChargedStep = step;
  stepMaxProcess->SetStepMax(MaxChargedStep);
}

StepMax* PhysicsList::GetStepMaxProcess()
{
  return stepMaxProcess;
}

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (stepMaxProcess->IsApplicable(*particle) && !particle->IsShortLived())
      {
         if (pmanager) pmanager ->AddDiscreteProcess(stepMaxProcess);
      }
  }
}

void PhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
   opticalPhysics->SetNbOfPhotonsCerenkov(MaxNumber);
}

void PhysicsList::SetVerbose(G4int verbose)
{
   opticalPhysics->GetCerenkovProcess()->SetVerboseLevel(verbose);
   opticalPhysics->GetScintillationProcess()->SetVerboseLevel(verbose);
   opticalPhysics->GetAbsorptionProcess()->SetVerboseLevel(verbose);
   opticalPhysics->GetRayleighScatteringProcess()->SetVerboseLevel(verbose);
   opticalPhysics->GetMieHGScatteringProcess()->SetVerboseLevel(verbose);
   opticalPhysics->GetBoundaryProcess()->SetVerboseLevel(verbose);
}
