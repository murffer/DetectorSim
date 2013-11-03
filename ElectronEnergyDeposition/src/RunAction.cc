#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "Analysis.hh"

#include "Randomize.hh"


/**
 * User hook into the run of the GEANT4 simulation, mostly for obtaining
 * analysis.
 *
 * @param det - the geometry of the simulation
 * @param kin - primary generator of this simulation
 */
RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
:fDetector(det), fPrimary(kin)
{
    
}


RunAction::~RunAction()
{
 
}

/**
 * Preforms the intilization of the run by setting the number of tracks,
 * secondaries, and ranges
 *
 * @param aRun - the run object
 */
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    Analysis* analysis = Analysis::GetInstance();
    analysis->PrepareNewRun(aRun);

}


/**
 * Summerizes the run and prints the output along with saving the histograms
 *
 * @param aRun - the run object
 */
void RunAction::EndOfRunAction(const G4Run* aRun)
{
    
    /* Getting run information */
    G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4String partName = particle->GetParticleName();
    G4double energy   = fPrimary->GetParticleGun()->GetParticleEnergy();
    G4Material* material = fDetector->GetMaterial();
    
    /* Writing the output to a file */
    Analysis* analysis = Analysis::GetInstance();
    analysis->EndOfRun(aRun);
    analysis->WriteRun(partName,material->GetName(),energy);
}
