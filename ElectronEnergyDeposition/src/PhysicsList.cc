/**
 * Sets the physics list of the simulation
 *
 * The physics list are electromagnetic based, using the G4 Kernal defaults
 */
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
 
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"

#include "DetectorConstruction.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

/**
 * Creates the default PhysicsList. The default list is the Livermore physics.
 *
 * @param p - the detector construction
 *
 */
PhysicsList::PhysicsList(DetectorConstruction* p) 
: G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  fDet = p;
  
  fCurrentDefaultCut   = 1.0*mm;              /* The default range cut  */
  fCutForGamma         = fCurrentDefaultCut;  /* Default gamma cut      */
  fCutForElectron      = 10*nm;               /* Default electron cut   */
  fCutForPositron      = fCutForElectron;     /* Default positron cut   */
  fCutForAlpha         = fCurrentDefaultCut;  /* Default cut for alpha  */
  fCutForTriton        = fCurrentDefaultCut;  /* Default cut for triton */

  fMessenger = new PhysicsListMessenger(this);  /* Physics List Messenger   */

  SetVerboseLevel(1);

  // EM physics
  fEmName = G4String("emlivermore");
  fEmPhysicsList =  new G4EmLivermorePhysics();

}

/**
 * Deconstructor
 */
PhysicsList::~PhysicsList()
{
  delete fMessenger;
}


// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

/**
 * Constructs the particles
 *
 * Particles are: bosons, leptons, mesons, bayrons, and nuclei. The bosons are 
 * for the gammas, the leptons for the electrons, the bayrons for protons and 
 * neturons, and the nuclei for the triton and alpha.
 */
void PhysicsList::ConstructParticle()
{
// pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
// gamma
  G4Gamma::GammaDefinition();
  
// optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();

// leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();  

// mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();

// barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

// ions
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();
}

/**
 * Adds Processes to the particles
 */
#include "G4EmProcessOptions.hh"
void PhysicsList::ConstructProcess()
{
  // Transportation
  //
  AddTransportation();

  // Electromagnetic physics list
  //
  fEmPhysicsList->ConstructProcess();
  
  // Em options
  //
  G4EmProcessOptions emOptions;
  emOptions.SetBuildCSDARange(true);
  emOptions.SetDEDXBinningForCSDARange(10*10);
    
  // Decay Process
  //
  //AddDecay();
    
  // Decay Process
  //
  //AddRadioactiveDecay();  

}

/**
 * Adds a physics list to the default
 *
 * @param name - name of the physics list
 */
void PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }
  
  if (name == fEmName) return;

    
  if (name == "emstandard_opt0") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics();

  } else if (name == "emstandard_opt1") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1();

  } else if (name == "emstandard_opt2") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2();

  } else if (name == "emstandard_opt3") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3();
    
  } else if (name == "emstandard_opt4") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();
    
  } else if (name == "emlivermore") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePhysics();
    
  } else if (name == "empenelope") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmPenelopePhysics();
            
  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}
/**
 * Sets up the decay process
 */
#include "G4ProcessManager.hh"
#include "G4Decay.hh"
void PhysicsList::AddDecay()
{ 
  // Decay Process
  //
  G4Decay* fDecayProcess = new G4Decay();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (fDecayProcess->IsApplicable(*particle)) { 

      pmanager ->AddProcess(fDecayProcess);

      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);

    }
  }
}

/**
 * Sets up the radioactive decay
 */
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
void PhysicsList::AddRadioactiveDecay()
{  
  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  radioactiveDecay->SetHLThreshold(-1.*s);
  radioactiveDecay->SetICM(true);                //Internal Conversion
  radioactiveDecay->SetARM(true);                //Atomic Rearangement
  
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
}


/**
 * Sets the default cuts for gamma, electrons, and positrons
 */
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
void PhysicsList::SetCuts()
{
  if (verboseLevel >0) {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");
  SetCutValue(fCutForAlpha, "alpha");
  SetCutValue(fCutForTriton, "triton");

  if (verboseLevel>0) DumpCutValuesTable();
}
/**
 * Sets the gamma cuts
 *
 * @param cut - the range for the cut
 */
void PhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

/**
 * Sets the triton cuts
 *
 * @param cut - the range for the cut
 */
void PhysicsList::SetCutForTriton(G4double cut)
{
  fCutForTriton = cut;
  SetParticleCuts(fCutForTriton, G4Triton::Triton());
}

/**
 * Sets the alpha cuts
 *
 * @param cut - the range for the cut
 */
void PhysicsList::SetCutForAlpha(G4double cut)
{
  fCutForAlpha = cut;
  SetParticleCuts(fCutForAlpha, G4Alpha::Alpha());
}

/**
 * Sets the electron cuts
 *
 * @param cut - the range for the cut
 */
void PhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}
/**
 * Sets the positron cuts
 *
 * @param cut - the range for the cut
 */
void PhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}
/**
 * Computes the CSDA range for electrons
 *
 * @param val - energy of the electron
 */
#include "G4Material.hh"
void PhysicsList::GetRange(G4double val)
{
  G4LogicalVolume* lBox = fDet->GetWorld()->GetLogicalVolume();
  G4ParticleTable* particleTable =  G4ParticleTable::GetParticleTable();
  const G4MaterialCutsCouple* couple = lBox->GetMaterialCutsCouple();
  const G4Material* currMat = lBox->GetMaterial();

  G4ParticleDefinition* part;
  G4double cut;
  part = particleTable->FindParticle("e-");
  cut = G4LossTableManager::Instance()->GetRange(part,val,couple);
  G4cout << "material : " << currMat->GetName()       << G4endl;
  G4cout << "particle : " << part->GetParticleName()  << G4endl;
  G4cout << "energy   : " << G4BestUnit(val,"Energy") << G4endl;
  G4cout << "range    : " << G4BestUnit(cut,"Length") << G4endl;
}
