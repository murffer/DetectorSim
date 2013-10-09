#include "HistoManager.hh"
#include "G4UnitsTable.hh"

/**
 * Creates a Histogram manger with the default name
 */
HistoManager::HistoManager(): fFileName("EnergyDepTrack")
{
  Book();
}
/**
 * Deconstructor
 */
HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}
/**
 * Sets up the histograms
 *
 * The histograms are inactived as the number of bins and the range of the
 * histograms have not been set.  The type of analysis is done by the selection
 * of the namespace in HistoManger.hh
 */
void HistoManager::Book()
{
  // Create or get analysis manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->SetFileName(fFileName);
  man->SetVerboseLevel(1);
  man->SetFirstHistoId(1);     // start histogram numbering from 1
  man->SetActivation(true);    // enable inactivation of histograms
  

  // Create all histograms as inactivated 
 G4int ih = man->CreateH1(1,"Energy Deposition",100,0*MeV,10*MeV);
 man->SetActivation(G4VAnalysisManager::kH1, ih, false);
 ih = man->CreateH2(2,"Positional Energy Depositon",100,0*cm,10*cm,100,0*cm,10*cm); 
 man->SetActivation(G4VAnalysisManager::kH2, ih, false);
 
 // Creating Ntuple
 ih = man->CreateNtuple("0", "Energy Deposition and Position");
 man->CreateNtupleDColumn("EDep");
 man->CreateNtupleDColumn("x");
 man->CreateNtupleDColumn("y");
 man->CreateNtupleDColumn("z");
 man->FinishNtuple();
 man->SetActivation(G4VAnalysisManager::kNtuple, ih, false);
}
