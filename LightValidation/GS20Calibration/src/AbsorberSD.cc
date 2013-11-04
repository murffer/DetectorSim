#include "AbsorberSD.hh"
#include "AbsorberHit.hh"
#include "PhotonHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

AbsorberSD::AbsorberSD(const G4String& name,const G4String& HCname) :
  G4VSensitiveDetector(name),hitCollection(NULL) 
{

  collectionName.insert(HCname);
}


AbsorberSD::~AbsorberSD(){ }

/**
 * Initialize
 *
 * Creates the hit collection of this event
 */
void AbsorberSD::Initialize(G4HCofThisEvent* HCE){

  // Create Hits Collection
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hitCollection = new AbsHitsCollection(SensitiveDetectorName,collectionName[0]); 
  HCE->AddHitsCollection( HCID, hitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool AbsorberSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){

  // Making sure step exits
  if (aStep == NULL) return false;

  // Creating a new hit
  AbsorberHit* newHit = new AbsorberHit();
  newHit->SetEdep		    (aStep->GetTotalEnergyDeposit());
  newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
  newHit->SetMomentum	  (aStep->GetPreStepPoint()->GetMomentum());
  newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
  newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
  newHit->SetArrivalTime(aStep->GetTrack()->GetGlobalTime());
  hitCollection->insert( newHit );
  return true;
}
/**
 * EndOfEvent - called for the hit at the end of the event
 *
 */
void AbsorberSD::EndOfEvent(G4HCofThisEvent*){}

