#include "PMTSD.hh"
#include "PhotonHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

PMTSD::PMTSD(const G4String& name, const G4String& HCname) :
    G4VSensitiveDetector(name),hitCollection(NULL) {
        collectionName.insert(HCname);
    }


PMTSD::~PMTSD(){ }


void PMTSD::Initialize(G4HCofThisEvent* HCE){

    // Create Hits Collection
    hitCollection = new PhotonHitsCollection(SensitiveDetectorName,collectionName[0]);
    
    static G4int HCID = -1;
    if (HCID<0) HCID = GetCollectionID(0);
    HCE->AddHitsCollection( HCID,hitCollection);
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool PMTSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){
    // Making sure step exits
    if (aStep == NULL) return false;

    // Making sure it is an optical photon
    G4Track* theTrack = aStep->GetTrack();
    if(theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()){return false;}
    // Setting hit informaiton
    PhotonHit* newHit = new PhotonHit();
    newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
    newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
    newHit->SetArrivalTime(theTrack->GetGlobalTime());
    hitCollection->insert( newHit );
    return true;
}


void PMTSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel > 1){
        G4int nOfHits = hitCollection->entries();
        G4cout<<"\n-----> Hits Collection: in this event they are "<<
            nOfHits<<" hits:"<<G4endl;
        for (G4int i=0; i<nOfHits; i++) (*hitCollection)[i]->Print();
    }
}

