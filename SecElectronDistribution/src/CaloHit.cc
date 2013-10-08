#include "CaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<CaloHit> HitAllocator;

CaloHit::CaloHit(): G4VHit(){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of CaloHit
 */
CaloHit::~CaloHit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void CaloHit::Print(){
	G4cout <<"Hit: "<<this
         <<"\n\ttrackID: "<<trackID
         <<"\n\tparentID: "<<parentID
         <<"\n\tparticle: "<<particle->GetParticleName()
		     <<"\n\tcreator process: "<<creatorName
         <<"\n\tpost process name: "<<postProcessName
         <<"\n\tfirst step in volume: "<<firstStep
//       <<"\n\tvolume: "<<volume->GetName()
//         <<"\n\tposition: " << G4BestUnit(pos,"Length")
         <<"\n\tKinetic Energy: "<<G4BestUnit(kEnergy,"Energy")
		     <<"\n\tenergy deposit: "<<G4BestUnit(edep,"Energy")
//         <<"\n\tstep length: "<< G4BestUnit(stepLength,"Length")
         <<G4endl;
}

