#include "SensitiveDetector.hh"
#include "CaloHit.hh"
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


    CaloHit* newCaloHit = new CaloHit();
    newCaloHit->SetTrackID	(aStep->GetTrack()->GetTrackID());
    newCaloHit->SetParentID    (aStep->GetTrack()->GetParentID());
    newCaloHit->SetEdep		(edep);
    newCaloHit->SetStepLength	(stepLength);
    newCaloHit->SetPosition	(aStep->GetPreStepPoint()->GetPosition());
    newCaloHit->SetMomentum	(aStep->GetPreStepPoint()->GetMomentum());
    newCaloHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
    newCaloHit->SetParticle   (aStep->GetTrack()->GetDefinition());
    newCaloHit->SetVolume		(aStep->GetTrack()->GetVolume());
    newCaloHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    hitCollection->insert( newCaloHit );

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

