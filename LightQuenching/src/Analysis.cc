#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"
#include "TMath.h"
#include "TAxis.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "G4Event.hh"
#include "G4ThreeVector.hh"

#include "PhotonHit.hh"
#include "AbsorberHit.hh"

Analysis* Analysis::singleton = 0;

/**
 * Analysis
 *
 * Creates an Analysis object 
 */
Analysis::Analysis(){
  incidentParticleName = "";
  outfile = NULL;
  eDepHist = opAbsHist = opPMTHist = NULL;

  maxHistEnergy = 5*MeV;
}
Analysis::~Analysis(){
  G4cout<<"Deleting the analysis object"<<G4endl;
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 * There is no need to update the geometry because the geometry
 * is fixed during a run.
 */
void Analysis::PrepareNewRun(const G4Run* aRun){

  // Getting the detector thickness
  G4String detMat = GetDetectorMaterial();
  G4int maxNumPhotons = 100000;

  std::ostringstream oss;
  oss <<incidentParticleName<<"_"<<detMat<<".root";
  std::string fname = oss.str();
  fname.erase(remove(fname.begin(), fname.end(),' '),fname.end());

  // Creating ROOT analysis objects (histogram)
  outfile = new TFile(fname.data(),"RECREATE");
  
  // Creating ROOT Analysis Objects
  eDepHist = new TH1F("eDepHist","Total Energy Deposition",100,0*eV,maxHistEnergy);
  opAbsHist = new TH1F("opAbsHist","Optical Photons Created",100,0,maxNumPhotons);
  opPMTHist = new TH1F("opPMTHist","Optical Photons Created",100,0,maxNumPhotons);

  G4cout<<"Prepared analysis for run "<<aRun->GetRunID()<<G4endl;
}
/**
 * Sets the number of optical photons generated
 */
void Analysis::SetNumOpticalPhotonsGenerated(G4int numPhotons){
  nOPAbsEvent = numPhotons;
}
/**
 * GetDetectorMaterial
 */
G4String Analysis::GetDetectorMaterial(){
  G4LogicalVolume* detLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Absorber");
  G4Material* detMat = NULL;
  if ( detLV) {
    detMat = dynamic_cast< G4Material*>(detLV->GetMaterial()); 
  } 
  if (detMat)
    return detMat->GetName();
  else
    return G4String("UNKOWN");
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event. Sets the energy deposited per event to be
 * zero, as well as the number of optical photons that are created and detected
 */
void Analysis::PrepareNewEvent(const G4Event*){
  eDepEvent = 0.0;
  nOPAbsEvent = 0;
  nOPPMTEvent = 0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  /**
   * Getting the hit collections. 
   * The PMT SD has a single hit collection associated with it, while the
   * absorber SD has two hit collections assciated with it.
   */
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  G4int pmtHCID = fSDM->GetCollectionID("PMTHitCollection");
  G4int absHCID = fSDM->GetCollectionID("AbsHitCollection");
  G4HCofThisEvent* HCofEvent = event->GetHCofThisEvent();
 
  AbsHitsCollection* absHC = (AbsHitsCollection*) (HCofEvent->GetHC(absHCID));
  PhotonHitsCollection* pmtHC = (PhotonHitsCollection*) (HCofEvent->GetHC(pmtHCID));

  // The Number of Optical Photons Detected on the PMT
  if(pmtHC != NULL)
    nOPPMTEvent += pmtHC->GetSize();
  else
    G4cout<<"WARNING: Absorber::EndOfEvent - pmtHC is NULL"<<G4endl;
  
  // Average Energy Deposition and Optical Photons Generated
  AbsorberHit *absHit;
  for (unsigned int i = 0; i < absHC->GetSize(); i++){
    absHit =  (AbsorberHit*) absHC->GetHit(i);
      eDepEvent += absHit->GetEdep()/MeV;
  }
  // Filling the histograms
  if (eDepEvent > 0.0){
    eDepHist->Fill(eDepEvent);
    opAbsHist->Fill(nOPAbsEvent);
    opPMTHist->Fill(nOPPMTEvent);
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* ){
  // Print out some run statistics
  G4cout<<"----> RUN SUMMARY"
        <<"\n\tAverage Energy Depostion: "<<eDepHist->GetMean()<<" +/- "<<eDepHist->GetMeanError()
        <<"\n\tAverage Number of Optical Photons Created: "<<opAbsHist->GetMean()<<" +/- "<<opAbsHist->GetMeanError()
        <<"\n\tAverage Number of Optical Photons Detected: "<<opPMTHist->GetMean()<<" +/- "<<opPMTHist->GetMeanError()
        <<"\n\tOptical Photon Collection Efficiency: "<<opPMTHist->GetMean()/opAbsHist->GetMean()<<" +/- "
        <<sqrt((pow(opAbsHist->GetMeanError()/opAbsHist->GetMean(),2)+pow(opPMTHist->GetMeanError()/opPMTHist->GetMean(),2))*opPMTHist->GetMean()/opAbsHist->GetMean())
        <<G4endl; 
  outfile->Write();
  outfile->Close();
  delete outfile;
}

/**
 * SetIncidentParticleName
 *
 * Sets the incident particle name (for the messener / file name)
 */
void Analysis::SetIncidentParticleName(G4String pName){
  incidentParticleName = pName;

  if (incidentParticleName == "neutron" )
    neutron = true;
  else
    neutron = false;
}

