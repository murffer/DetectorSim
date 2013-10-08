#include "CaloSensitiveDetector.hh"
#include "CaloHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"

CaloSensitiveDetector::CaloSensitiveDetector(const G4String& name,
        const G4String& HCname) :
    G4VSensitiveDetector(name),hitCollection(NULL) {

        collectionName.insert(HCname);
    }


CaloSensitiveDetector::~CaloSensitiveDetector(){ }


void CaloSensitiveDetector::Initialize(G4HCofThisEvent* HCE){

    // Create Hits Collection
    hitCollection = new CaloHitsCollection(SensitiveDetectorName,collectionName[0]); 
    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection( HCID, hitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool CaloSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*){

    CaloHit* newHit = new CaloHit();
    //G4cout<<"First step?: "<<aStep->IsFirstStepInVolume()<<G4endl;
    const G4VProcess *p = aStep->GetTrack()->GetCreatorProcess();
    G4String pName = "FirstStep";
    if (p != NULL)
      pName = p->GetProcessName();
    newHit->SetCreatorProcess(pName);
    pName = "";
    p = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    if (p != NULL)
      pName = p->GetProcessName();
    newHit->SetFirstStep(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary);
    
    newHit->SetPostProcess(pName);
    newHit->SetTrackID    (aStep->GetTrack()->GetTrackID());
    newHit->SetParentID   (aStep->GetTrack()->GetParentID());
    newHit->SetEdep		    (aStep->GetTotalEnergyDeposit());
    newHit->SetStepLength	(aStep->GetStepLength());
    newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
    newHit->SetMomentum	  (aStep->GetPreStepPoint()->GetMomentum());
    newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
    newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
    newHit->SetVolume		  (aStep->GetTrack()->GetVolume());
    hitCollection->insert( newHit );

    return true;
}


void CaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel > 1){
        G4int nOfHits = hitCollection->entries();
        G4cout<<"\n-----> Hits Collection: in this event they are "<<
            nOfHits<<" hits:"<<G4endl;
        for (G4int i=0; i<nOfHits; i++) (*hitCollection)[i]->Print();
    }
}

