#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "Randomize.hh"
#include <iomanip>

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

  fEdep = fEdep2 = 0.;
    
}

/**
 * Summerizes the run and prints the output along with saving the histograms
 *
 * @param aRun - the run object
 */
void RunAction::EndOfRunAction(const G4Run* aRun)
{ 
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  G4double dNbOfEvents = double(NbOfEvents);
    
  G4ParticleDefinition* particle = fPrimary->GetParticleGun()
                                          ->GetParticleDefinition();
  G4String partName = particle->GetParticleName();    
  G4double energy   = fPrimary->GetParticleGun()->GetParticleEnergy();
  
  G4double length      = fDetector->GetSize();    
  G4Material* material = fDetector->GetMaterial();
  G4double density     = material->GetDensity();
   
  G4cout << "\n ======================== run summary ======================\n";
  
  G4int prec = G4cout.precision(5);
  
  G4cout << "\n The run was: " << NbOfEvents << " " << partName << " of "
         << G4BestUnit(energy,"Energy") << " through " 
         << G4BestUnit(length,"Length") << " of "
         << material->GetName() << " (density: " 
         << G4BestUnit(density,"Volumic Mass") << ")\n" << G4endl;


    G4double eDepRms;

    fEdep /= dNbOfEvents; fEdep2 /= dNbOfEvents;
    eDepRms = fEdep2 - fEdep*fEdep;
    if (eDepRms>0.) eDepRms = std::sqrt(eDepRms); else eDepRms = 0.;
    
 G4cout << "\n total energy deposit: " 
        << G4BestUnit(fEdep, "Energy")
        << "+/- "<<G4BestUnit(eDepRms,"Energy")
        <<G4endl;
    
    
    G4cout << "\n ============================================================\n";
    
    // reset default precision
 G4cout.precision(prec);

}
