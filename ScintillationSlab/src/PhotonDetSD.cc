
#include "PhotonDetSD.hh"
#include "PhotonDetHit.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

PhotonDetSD::PhotonDetSD(G4String name)
  : G4VSensitiveDetector(name),PhotonDetHitCollection(0)
{
  collectionName.insert("PhotonDetHitCollection");
}

PhotonDetSD::~PhotonDetSD() { }

void PhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
  PhotonDetHitCollection =
       new PhotonDetHitsCollection(SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int HCID = -1;
  if (HCID<0) HCID = GetCollectionID(0);
  HCE->AddHitsCollection( HCID, PhotonDetHitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool PhotonDetSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){
    if (aStep == NULL) return false;
    G4Track* theTrack = aStep->GetTrack();
    
    // Need to know if this is an optical photon
    if(theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
    
    // Find out information regarding the hit
    G4double      arrivalTime  = theTrack -> GetGlobalTime();
    
    // Creating the hit and add it to the collection
    PhotonDetHitCollection->insert(new PhotonDetHit(arrivalTime));
    return true;
}


void PhotonDetSD::EndOfEvent(G4HCofThisEvent* ){ }

void PhotonDetSD::clear(){ }

void PhotonDetSD::DrawAll(){ }

void PhotonDetSD::PrintAll(){ }
