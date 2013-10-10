#include "Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<Hit> HitAllocator;

Hit::Hit(): G4VHit(){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of Hit
 */
Hit::~Hit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void Hit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "
         <<particle->GetParticleName()<<" rank: "<<particleRank
         <<" chamber number: "<<chamberNum
		     <<"\nvolume: "<<volume->GetName()
         <<" position[mm]: " << pos/mm
         <<"\n Momentum "<<momentum<<" Kinetic Energy[keV]"<<kEnergy/keV
		     <<"\nenergy deposit[keV]: " << edep/keV<<" step length [mm]: "
         << stepLength/mm
         <<G4endl;
}

