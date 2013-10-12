/**
 * @file    RunAction.hh
 *
 * @date    5 July 2012
 * @author  murffer
 *
 * @brief   Implements user class RunAction
 */
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "HistoManager.hh"
/**
 * User hook for the run
 *
 */
RunAction::RunAction() : G4UserRunAction(){
  // Creating the histogram mananager
  fHistoManager = new HistoManager();
}
/**
 * Deconstructo
 */
RunAction::~RunAction(){
   delete fHistoManager;
}
/**
 * Called at the beginning of the run
 *
 * Opens the histograms
 */
void RunAction::BeginOfRunAction(const G4Run* run){ 
  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }   
	G4cout<<"Starting  run: " << run->GetRunID()<< G4endl;
}
/**
 * Called at the end of the run
 *
 * Writes the histogram and some summary data
 */
void RunAction::EndOfRunAction(const G4Run* aRun){
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
    G4cout << "Average Number of Optical Photons: " 
      << analysisManager->GetH1(1)->mean() 
      << " +/- " << analysisManager->GetH1(1)->rms()
      << G4endl;
  
  //save histograms      
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }    
	G4cout<<"Ending run: " << aRun->GetRunID()<< G4endl;
}
