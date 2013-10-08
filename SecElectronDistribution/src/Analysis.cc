#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
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

#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#ifdef G4MPIUSE
#include "G4MPImanager.hh"
#endif

Analysis* Analysis::singleton = 0;

/**
 * Analysis
 *
 * Creates an Analysis object 
 */
Analysis::Analysis(){
  // Empty Constructor, assingment done in constuctor list
  incidentParticleName = "";
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
  G4double detThickness = GetDetectorThickness();
  G4String detMat = GetDetectorMaterial();

  std::ostringstream oss;
  oss <<incidentParticleName<<"_"
    <<detMat<<"_"<<G4BestUnit(detThickness,"Length")<<".root";
  std::string fname = oss.str();
  fname.erase(remove(fname.begin(), fname.end(),' '),fname.end());

  // Creating ROOT analysis objects (histogram)
  outfile = new TFile(fname.data(),"RECREATE");
  G4double histMax = 1.25*MeV;
  kinEHist = TH1FLog("kinEHist","Secondary Electron Kinetic Energy",100,0*eV,histMax);
  kinETuple = new TNtuple("kinETuple","Kinetic Energy Tuple","kinE");
  numSecHist = new TH1F("numSecHist","Number of Secondary Electrons",150,0,150); 
  if (neutron){
    aKinETuple = new TNtuple("aKinETuple","Alpha Kinetic Energy Tuple","kinE");
    tKinETuple = new TNtuple("tKinETuple","Triton Kinetic Energy Tuple","kinE");
    kEAlphaHist = new TH1F("kEAlphaHist","Secondary Electron Kinetic Energy",100,0.9,2.0);
    kEAlphaHist->GetXaxis()->SetTitle("Kinetic Energy (keV)");
    kETritonHist = new TH1F("kETritonHist","Secondary Electron Kinetic Energy",100,0.9,2.0);
    kETritonHist->GetXaxis()->SetTitle("Kinetic Energy (keV)");
    nSAlphaHist = new TH1F("nSAlphaHist","Number of Secondary Electrons",150,0,150); 
    nSTritonHist = new TH1F("nSTritonHist","Number of Secondary Electrons",150,0,150); 
    kEAlphaHistLog = TH1FLog("kEAlphaHistLog","Secondary Electron Kinetic Energy",100,0*eV,histMax);
    kETritonHistLog = TH1FLog("kETritonHistLog","Secondary Electron Kinetic Energy",100,0*eV,histMax);
  }
  G4cout<<"Prepared run "<<aRun->GetRunID()<<G4endl;
}
/**
 * TH1FLog
 *
 * @brief Setting up logarithimic binning - Wrapper for TH1F
 * @brief name - root key name
 * @brief title - title of the histogram
 * @brief numBins - number of bins
 * @double xMin - minimum for x
 * @double xMax - maximum for x
 */
TH1F* Analysis::TH1FLog(const char *name,const char* title, int numBins, double xMin,double xMax){
  
  // Setting up the log axis
  if (xMin ==0.0)
    xMin = 100*eV;
  double logxmin = TMath::Log10(xMin);
  double logxmax = TMath::Log10(xMax);
  double binwidth = (logxmax-logxmin)/numBins;
  double* xbins = new double[numBins+1];

  xbins[0] = xMin;
  for(int i = 0; i <= numBins; i++){
    xbins[i] = xMin + TMath::Power(10,logxmin+i*binwidth);
  }
  return new TH1F(name,title,numBins,xbins);
}
/**
 * MyFill
 */
void Analysis::MyFill(TH1F* h, double val){
  if (val > 0.0)
    h->Fill(val);
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
 * GetDetectorThickness
 * @return the thickness of the detector, from the G4LogicalVolumeStore
 */
G4double Analysis::GetDetectorThickness(){
  G4double detThickness = 0;
  G4LogicalVolume* detLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Absorber");
  G4Tubs* detTubs = 0;
  if ( detLV) {
    detTubs = dynamic_cast< G4Tubs*>(detLV->GetSolid()); 
  } 
  if ( detTubs ) {
    detThickness = detTubs->GetZHalfLength()*2;  
  }
  else  {
    G4cerr << "Detector Thickness not found." << G4endl;
  } 
  return detThickness;
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 */
void Analysis::PrepareNewEvent(const G4Event* event){
  numSecTriton = 0;
  numSecAlpha = 0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  G4VHitsCollection *hc;
  CaloHit *hit;

  // Iterating through the hit collection to accumulate the energy deposition 
  G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
  G4double kinE = 0.0;
  for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
    // Itterating through the hit collection
    hc = event->GetHCofThisEvent()->GetHC(hitItter);
    for(G4int i = 0; i < hc->GetSize(); i++){
      hit = (CaloHit*) hc->GetHit(i);
      G4int parentID = hit->GetParentID();
      if (hit->GetParticle()->GetParticleName() != "e-")
        continue;
      // The secondary electrons from a charged particle  
      if (neutron && (parentID == 2 || parentID == 3)){
        kinE = hit->GetKineticEnergy();
        kinETuple->Fill(kinE);
        MyFill(kinEHist,kinE);
        if (parentID == 2){ // triton
          tKinETuple->Fill(kinE);
          kETritonHist->Fill(kinE/keV);
          MyFill(kETritonHistLog,kinE);
          numSecTriton += 1;
        }
        else{ // alpha
          numSecAlpha += 1;
          kEAlphaHist->Fill(kinE/keV);
          MyFill(kEAlphaHistLog,kinE);
          aKinETuple->Fill(kinE);
        }
      }
      // The secondary electron from a Gamma Event
      else if (!neutron && parentID == 1){
        if (hit->GetKineticEnergy() > kinE)
          kinE = hit->GetKineticEnergy();
      }
    }
  }
  // End of Itterating through hit collection
  if (neutron){
    nSAlphaHist->Fill(numSecAlpha);
    nSTritonHist->Fill(numSecTriton);
    numSecHist->Fill(numSecTriton+numSecAlpha);
  }
  else{
    // Gamma Filling with first energy
    MyFill(kinEHist,kinE);
    kinETuple->Fill(kinE);
    numSecHist->Fill(1);
  }
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){

  // Lets get the number of events simulated
  G4int numEvents = aRun->GetNumberOfEventToBeProcessed();
  G4double detThickness = GetDetectorThickness();
  G4cout.precision(5);
  G4cout<<"End Of Run "<<aRun->GetRunID()
    <<"\n\tRan "<<numEvents<<" events"
    <<"\n\tThickness: "<<G4BestUnit(detThickness,"Length")
    <<"\n\tAverage Kinetic Energy of Secondaries: "<<std::setw(6)<<G4BestUnit(kinEHist->GetMean(),"Energy")
    <<" +/- "<<std::setw(6)<<G4BestUnit(kinEHist->GetMeanError(),"Energy")
    <<"\n\tAverage number of Secondaries: "<<std::setprecision(5)<<numSecHist->GetMean()
    <<" +/- "<<std::setprecision(5)<<numSecHist->GetMeanError()<<G4endl;
  if(neutron){
    G4cout<<"\t         Average Secondary Energy [keV]      Average Number of Secondaries"
      <<"\n\tAlpha "<<std::setw(10)<<G4BestUnit(kEAlphaHist->GetMean(),"Energy")<<" +/- "<<std::setw(6)<<G4BestUnit(kEAlphaHist->GetMeanError(),"Energy")
      <<"      "<<std::setw(6)<<nSAlphaHist->GetMean()<<" +/- "<<std::setw(10)<<nSAlphaHist->GetMeanError()
      <<"\n\tTriton"<<std::setw(10)<<G4BestUnit(kETritonHist->GetMean(),"Energy")<<" +/- "<<std::setw(6)<<G4BestUnit(kETritonHist->GetMeanError(),"Energy")
      <<"      "<<std::setw(6)<<nSTritonHist->GetMean()<<" +/- "<<std::setw(10)<<nSTritonHist->GetMeanError()
      <<G4endl;
  }
  G4cout.precision(5);
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

