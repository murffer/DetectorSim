#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "PhotonHit.hh"

class G4Step;
class G4HCofThisEvent;

class DetectorSD : public G4VSensitiveDetector
{
  public:
      DetectorSD(const G4String& name, const G4String& HCname);
     ~DetectorSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      PhotonHitsCollection* hitCollection;

};

#endif

