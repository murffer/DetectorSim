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

#include "Randomize.hh"
#include <iomanip>
#include <iostream>
#include <fstream>

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
    /* Getting the world size */
    G4double worldXHalfLength = 0.5*cm;
    G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box* worldBox = NULL;
    if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    if ( worldBox ) worldXHalfLength = -worldBox->GetXHalfLength();
    else  {
        G4cerr << "World volume of box not found." << G4endl;
        G4cerr << "Setting the detector size to be 1 cm"<<G4endl;
    }
    fWorldSize = worldXHalfLength*2;
    for (G4int i = 0; i < fNumBins; i++) {
        fEdep[i] = 0.0;
        fEdep2[i] = 0.0;
    }
}
/**
 * Increments the energy deposition for each run
 * @param xPos - the xPosition of the event
 * @param val - the energy deposition
 */
void RunAction::AddEdep(G4double xPos, G4double val){
    G4int index = (int) xPos/fBinWidth;
    fEdep[index] += val;
    fEdep2[index] += val*val;
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
    
    /* Getting run information */
    G4ParticleDefinition* particle = fPrimary->GetParticleGun()
    ->GetParticleDefinition();
    G4String partName = particle->GetParticleName();
    G4double energy   = fPrimary->GetParticleGun()->GetParticleEnergy();
    G4Material* material = fDetector->GetMaterial();
    
    
    
    /* Writing the output to a file */
    G4double eDepRms;
    std::ofstream myfile;
    std::ostringstream filename;
    filename << material->GetName()<<"_"<<energy/keV<<"keV.csv";
    myfile.open (filename.str());
    myfile <<"Position (um),Energy Deposited (keV),error"<<std::endl;
    
    for (G4int i = 0; i<fNumBins; i++) {
        fEdep[i]/=dNbOfEvents; fEdep2[i] /= dNbOfEvents;
        eDepRms = fEdep2[i] - fEdep[i]*fEdep[i];
        if (eDepRms>0.) eDepRms = std::sqrt(eDepRms); else eDepRms = 0.;
        myfile<<i*fBinWidth/um<<","<<fEdep[i]/keV<<","<<eDepRms/keV<<std::endl;
    }
    myfile.close();
    
    /* Cleaning up memory */
    delete fEdep;
    delete fEdep2;
    
}
