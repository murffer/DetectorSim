#ifndef AbsorberSD_h
#define AbsorberSD_h 1

#include "G4VSensitiveDetector.hh"
#include "AbsorberHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class AbsorberSD : public G4VSensitiveDetector
{
  public:
      AbsorberSD(const G4String& name, const G4String& HCname);
     ~AbsorberSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      G4bool ProcessAbsorberHit(G4Step*,G4TouchableHistory*);
      G4bool ProcessOpticalPhotonHit(G4Step*,G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      AbsHitsCollection* hitCollection;
};

#endif

