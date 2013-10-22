
#include "PhotonDetSD.hh"
#include "PhotonDetHit.hh"
#include "UserTrackInformation.hh"

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

G4bool PhotonDetSD::ProcessHits(G4Step* , G4TouchableHistory* )
{
  return false;
}

//Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//is generated manually when the photon hits the detector
G4bool PhotonDetSD::ProcessHits_constStep(const G4Step* aStep,
                                             G4TouchableHistory* )
{
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // Need to know if this is an optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  // Find out information regarding the hit
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
 
  UserTrackInformation* trackInformation
      = (UserTrackInformation*)theTrack->GetUserInformation();
 
  G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(thePostPoint->GetTouchable());
 
  G4ThreeVector photonExit   = trackInformation -> GetExitPosition();
  G4ThreeVector photonArrive = thePostPoint -> GetPosition();
  G4double      arrivalTime  = theTrack -> GetGlobalTime();

  // Convert the global coordinate for arriving photons into
  // the local coordinate of the detector
  photonArrive = theTouchable->GetHistory()->
                                GetTopTransform().TransformPoint(photonArrive);

  // Creating the hit and add it to the collection
  PhotonDetHitCollection->
            insert(new PhotonDetHit(photonExit, photonArrive, arrivalTime));

  return true;
}

void PhotonDetSD::EndOfEvent(G4HCofThisEvent* ){ }

void PhotonDetSD::clear(){ }

void PhotonDetSD::DrawAll(){ }

void PhotonDetSD::PrintAll(){ }
