 #ifndef MyIonPhysics_h
 #define MyIonPhysics_h 1
 
 #include "globals.hh"
 
 #include "G4VPhysicsConstructor.hh"
 
 class  G4DeuteronInelasticProcess;
 class  G4LEDeuteronInelastic;
 class  G4TritonInelasticProcess;
 class  G4LETritonInelastic;
 class  G4AlphaInelasticProcess;
 class  G4LEAlphaInelastic;
 
 class MyIonPhysics : public G4VPhysicsConstructor
 {
 public:
   MyIonPhysics(G4int verbose =1);
   //obsolete
   MyIonPhysics(const G4String& name);
   virtual ~MyIonPhysics();
 
   // This method will be invoked in the Construct() method.
   // each particle type will be instantiated
   virtual void ConstructParticle();
 
   // This method will be invoked in the Construct() method.
   // each physics process will be instantiated and
   // registered to the process manager of each particle type
   virtual void ConstructProcess();
 
 private:
 
   // Deuteron physics
   G4DeuteronInelasticProcess* fDeuteronProcess;
   G4LEDeuteronInelastic*      fDeuteronModel;
 
   // Triton physics
   G4TritonInelasticProcess*   fTritonProcess;
   G4LETritonInelastic*        fTritonModel;
 
   // Alpha physics
   G4AlphaInelasticProcess*    fAlphaProcess;
   G4LEAlphaInelastic*         fAlphaModel;
 
 
   G4bool wasActivated;
 };
 #endif
