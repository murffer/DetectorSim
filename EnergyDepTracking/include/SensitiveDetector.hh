#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "CaloHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class CaloHit;

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
      SensitiveDetector(const G4String& name, const G4String& HCname);
     ~SensitiveDetector();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      HitsCollection* hitCollection;

};

#endif

