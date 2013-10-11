/**
 * Sets the physics list of the simulation
 *
 * The physics list are electromagnetic based, using the G4 Kernal defaults
 */
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
 
#include "G4EmLivermorePhysics.hh"

#include "DetectorConstruction.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

/**
 * Creates the default PhysicsList. The default list is the Livermore physics.
 *
 *
 */
PhysicsList::PhysicsList() 
: G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  
  fCurrentDefaultCut   = 1*um;              /* The default range cut  */
  fCutForGamma         = fCurrentDefaultCut;  /* Default gamma cut      */
  fCutForElectron      = fCurrentDefaultCut;  /* Default electron cut   */
  fCutForPositron      = fCurrentDefaultCut;  /* Default positron cut   */
  fCutForAlpha         = 0.1*um;  /* Default cut for alpha  */
  fCutForTriton        = 0.5*um;  /* Default cut for triton */

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
