#include "PhotonHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<PhotonHit> PhotonHitAllocator;
/**
 * Creates a PhotonHit
 *
 * Hit of a photon on the detector
 */
PhotonHit::PhotonHit(): G4VHit(){
  pos = G4ThreeVector();
  arrivalTime = 0;
  kEnergy = 0;
}

/**
 * Destroctor of PhotonHit
 */
PhotonHit::~PhotonHit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void PhotonHit::Print(){
	G4cout << "OpticalPhoton"
         <<"\n\tArrival Time: "<<G4BestUnit(arrivalTime,"Time")
         <<"\n\tposition[mm]: " << pos/mm
         <<"\n\tkinetic energy "<<G4BestUnit(kEnergy,"Energy")
         <<G4endl;
}

