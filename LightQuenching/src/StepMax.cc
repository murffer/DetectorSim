#include "G4Track.hh"
#include "G4VParticleChange.hh"

#include "StepMax.hh"

StepMax::StepMax(const G4String& aName)
  : G4VDiscreteProcess(aName), MaxChargedStep(DBL_MAX)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}

StepMax::~StepMax() { }

StepMax::StepMax(StepMax& right) : G4VDiscreteProcess(right) { }

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0.);
}

void StepMax::SetStepMax(G4double step) { MaxChargedStep = step ; }

G4double StepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;

  if ( MaxChargedStep > 0.) ProposedStep = MaxChargedStep;

   return ProposedStep;
}

G4VParticleChange* StepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

G4double StepMax::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
  return 0.;
}
