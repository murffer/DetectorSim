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
  eDepHist = opAbsHist = opPMTHist = opPMTTopHist = opPMTBotHist = NULL;
  opLGHist = opLGTopHist = opLGBotHist = NULL;
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
  opPMTHist = new TH1F("opPMTTotHist","Optical Photons Detected (total)",100,0,maxNumPhotons);
  opPMTTopHist = new TH1F("opPMTTopHist","Optical Photons Detected (top)",100,0,maxNumPhotons);
  opPMTBotHist = new TH1F("opPMTBotHist","Optical Photons Detected (bottom)",100,0,maxNumPhotons);
  opLGHist = new TH1F("opLGHist","Optical Photons Detected (LG)",100,0,maxNumPhotons);
  opLGTopHist = new TH1F("opLGTopHist","Optical Photons Detected (LG top)",100,0,maxNumPhotons);
  opLGBotHist = new TH1F("opLGBotHist","Optical Photons Detected (LG bottom)",100,0,maxNumPhotons);

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
  nOPPMTTopEvent = 0;
  nOPPMTBotEvent = 0;
  nOPLGTopEvent = 0;
  nOPLGBotEvent = 0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  /**
   * Getting the hit collections. 
   */
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  G4int pmtTopHCID = fSDM->GetCollectionID("PMTTopHitCollection");
  G4int pmtBotHCID = fSDM->GetCollectionID("PMTBottomHitCollection");
  G4HCofThisEvent* HCofEvent = event->GetHCofThisEvent();
 
  PhotonHitsCollection* pmtHCTop = (PhotonHitsCollection*) (HCofEvent->GetHC(pmtTopHCID));
  PhotonHitsCollection* pmtHCBot = (PhotonHitsCollection*) (HCofEvent->GetHC(pmtBotHCID));

  // The Number of Optical Photons Detected on the PMT
  if(pmtHCTop != NULL && pmtHCBot != NULL){
    nOPPMTTopEvent += pmtHCTop->GetSize();
    nOPPMTBotEvent += pmtHCBot->GetSize();
    nOPPMTEvent += (nOPPMTTopEvent+nOPPMTBotEvent);
  }
  else
    G4cout<<"WARNING: Absorber::EndOfEvent - pmtHC is NULL"<<G4endl;
 
  // The number of optical photons detected in the light guide
  PhotonHitsCollection* LGHCTop = (PhotonHitsCollection*) (HCofEvent->GetHC(fSDM->GetCollectionID("LGTopHitCollection")));
  PhotonHitsCollection* LGHCBot = (PhotonHitsCollection*) (HCofEvent->GetHC(fSDM->GetCollectionID("LGBotHitCollection")));

  // Average Energy Deposition and Optical Photons Generated
  if (LGHCTop != NULL && LGHCBot != NULL){
    nOPLGTopEvent += LGHCTop->GetSize();
    nOPLGBotEvent += LGHCBot->GetSize();
    nOPLGEvent += (nOPLGTopEvent+nOPLGBotEvent);
  }
  else
    G4cout<<"WARNING: Absorber::EndOfEvent - LGHC is NULL"<<G4endl;
 /*
  G4int absHCID = fSDM->GetCollectionID("AbsHitCollection");
  AbsHitsCollection* absHC = (AbsHitsCollection*) (HCofEvent->GetHC(absHCID));
 AbsorberHit *absHit;
  for (unsigned int i = 0; i < absHC->GetSize(); i++){
    absHit =  (AbsorberHit*) absHC->GetHit(i);
      eDepEvent += absHit->GetEdep()/MeV;
  }
  // Filling the histograms
  if (eDepEvent > 0.0){
    eDepHist->Fill(eDepEvent);
  }
  */
   if (nOPAbsEvent > 0){
    opAbsHist->Fill(nOPAbsEvent);
    opPMTHist->Fill(nOPPMTEvent);
    opPMTTopHist->Fill(nOPPMTTopEvent);
    opPMTBotHist->Fill(nOPPMTBotEvent);
    opLGHist->Fill(nOPLGEvent);
    opLGTopHist->Fill(nOPLGTopEvent);
    opLGBotHist->Fill(nOPLGBotEvent);
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
        <<"\n\tAverage Number of Optical Photons Detected (LG) : "<<opLGHist->GetMean()<<" +/- "<<opPMTHist->GetMeanError()
        <<"\n\tAverage Number of Optical Photons Detected (PMT): "<<opPMTHist->GetMean()<<" +/- "<<opPMTHist->GetMeanError()
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

