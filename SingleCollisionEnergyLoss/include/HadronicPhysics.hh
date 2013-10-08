#ifndef HadronicPhysics_h
#define HadronicPhysics_h 1

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

// Elastic
#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"

// IO
#include "G4HadronInelasticProcess.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"

#include "G4HadronFissionProcess.hh"
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPFissionData.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"

class HadronicPhysics: public G4VPhysicsConstructor
{
  public:
    HadronicPhysics(const G4String& name="Neutron");
   ~HadronicPhysics();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess(); 


};

#endif



