#include "HadronicPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"

#include "G4ProcessManager.hh"
#include "G4LossTableManager.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "HadronPhysicsQGSC_BERT.hh"
#include "G4NeutronHPInelastic.hh"


HadronicPhysics::HadronicPhysics(const G4String& name) : G4VPhysicsConstructor (name)
{;}


HadronicPhysics::~HadronicPhysics(){ 
	;
	// Cleaning Up Memory
}
/**
*@brief ConstructProcess
*
* Constructs and assigns process to each particle
*/
void HadronicPhysics::ConstructProcess(){

	theParticleIterator->reset();
	while( (*theParticleIterator)() ){
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pManager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
		if (particleName == "neutron") {
			G4cout << " AddDiscreteProcess to " <<particleName<< G4endl;

			//Elastic Process
			G4HadronElasticProcess* neutronElasticProcess = new G4HadronElasticProcess();
			G4NeutronHPElastic* neutronHPElasticModel = new G4NeutronHPElastic();
			neutronHPElasticModel->SetMinEnergy(0.00001*eV);
			neutronHPElasticModel->SetMaxEnergy(10.*MeV);
			G4NeutronHPElasticData* neutronElasticData = new G4NeutronHPElasticData;
			neutronElasticProcess->AddDataSet(neutronElasticData);
			neutronElasticProcess->RegisterMe(neutronHPElasticModel);
			pManager->AddDiscreteProcess(neutronElasticProcess);

			// Inelastic Processes
			G4NeutronInelasticProcess* neutronInelasticProcess = new G4NeutronInelasticProcess(); 
			G4NeutronHPInelastic* neutronHPInelastic = new G4NeutronHPInelastic();
			neutronHPInelastic->SetMaxEnergy(10.*MeV);
			G4NeutronHPInelasticData* neutronInelasticData = new G4NeutronHPInelasticData;
			neutronInelasticProcess->AddDataSet(neutronInelasticData);
			neutronInelasticProcess->RegisterMe(neutronHPInelastic);
			pManager->AddDiscreteProcess(neutronInelasticProcess);

			// Fissions
			G4HadronFissionProcess* thenFission = new G4HadronFissionProcess();
			G4NeutronHPFission* thenHPFissionModel = new G4NeutronHPFission();
			thenHPFissionModel->SetMaxEnergy(10.*MeV);
			G4NeutronHPFissionData* neutronFissionData = new G4NeutronHPFissionData;
			thenFission->AddDataSet(neutronFissionData);
			thenFission->RegisterMe(thenHPFissionModel);
			pManager->AddDiscreteProcess(thenFission);

			// Captures
			G4HadronCaptureProcess* thenCapture = new G4HadronCaptureProcess();
			G4NeutronHPCapture* thenHPCaptureModel = new G4NeutronHPCapture();
			thenHPCaptureModel->SetMaxEnergy(20.*MeV);
			G4NeutronHPCaptureData* neutronCaptureData = new G4NeutronHPCaptureData;
			thenCapture->AddDataSet(neutronCaptureData);
			thenCapture->RegisterMe(thenHPCaptureModel);
			pManager->AddDiscreteProcess(thenCapture);
		}
	}
}



/**
* @brief ConstructParticle - Constructs all of the particles we want to use
*
* Only intrested in the bayrons
*/
void HadronicPhysics::ConstructParticle(){

	//  bayrons
	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();

	G4Neutron::NeutronDefinition();
	G4AntiNeutron::AntiNeutronDefinition();
}
