
#ifndef PhotonDetSD_h
#define PhotonDetSD_h 1

#include "PhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class PhotonDetSD : public G4VSensitiveDetector
{
  public:

    PhotonDetSD(G4String name);
    ~PhotonDetSD();

    void Initialize(G4HCofThisEvent* HCE);

    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* aStep,
                                 G4TouchableHistory* ROhist);

    void EndOfEvent(G4HCofThisEvent* HCE);

    void clear();
    void DrawAll();
    void PrintAll();

  private:

    PhotonDetHitsCollection* PhotonDetHitCollection;
 
};

#endif
