#ifndef ExtraPhysics_h
#define ExtraPhysics_h 1

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

class ExtraPhysics : public G4VPhysicsConstructor
{
  public:

    ExtraPhysics();
    virtual ~ExtraPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

};

#endif
