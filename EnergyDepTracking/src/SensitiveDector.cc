#include "SensitiveDetector.hh"
#include "Hit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"

SensitiveDetector::SensitiveDetector(const G4String& name,
        const G4String& HCname) :
    G4VSensitiveDetector(name),hitCollection(NULL) {

        collectionName.insert(HCname);
    }


SensitiveDetector::~SensitiveDetector(){ }


void SensitiveDetector::Initialize(G4HCofThisEvent* HCE){

    // Create Hits Collection
    hitCollection = new HitsCollection(SensitiveDetectorName,collectionName[0]); 
    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection( HCID, hitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool SensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*){

    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double stepLength = aStep->GetStepLength();

    // Only saving a hit if there was energy depostion
    G4double minEDep = 0.*eV;
    if ( edep <= minEDep || stepLength == 0.) return false;

    // Getting the copy number
    G4TouchableHistory* touchable = (G4TouchableHistory*)
        (aStep->GetPreStepPoint()->GetTouchable());


    Hit* newHit = new Hit();
    newHit->SetTrackID	(aStep->GetTrack()->GetTrackID());
    newHit->SetParentID    (aStep->GetTrack()->GetParentID());
    newHit->SetEdep		(edep);
    newHit->SetStepLength	(stepLength);
    newHit->SetPosition	(aStep->GetPreStepPoint()->GetPosition());
    newHit->SetMomentum	(aStep->GetPreStepPoint()->GetMomentum());
    newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
    newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
    newHit->SetVolume		(aStep->GetTrack()->GetVolume());
    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    hitCollection->insert( newHit );

    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel > 1){
        G4int nOfHits = hitCollection->entries();
        G4cout<<"\n-----> Hits Collection: in this event they are "<<
            nOfHits<<" hits:"<<G4endl;
        for (G4int i=0; i<nOfHits; i++) (*hitCollection)[i]->Print();
    }
}

