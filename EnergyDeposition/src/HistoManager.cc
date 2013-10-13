#include "HistoManager.hh"
#include "G4UnitsTable.hh"

/**
 * Creates a Histogram manger with the default name
 */
HistoManager::HistoManager(): fFileName("energyDep")
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
  G4cout<<"Creating histograms"<<G4endl;
  // Create or get analysis manager
  G4AnalysisManager* aMan = G4AnalysisManager::Instance();
  aMan->SetFileName(fFileName);
  aMan->SetVerboseLevel(1);
  aMan->SetFirstHistoId(1);     // start histogram numbering from 1
  aMan->SetActivation(true);    // enable inactivation of histograms
  

  // Create all histograms as inactivated 
    G4int ih = aMan->CreateH1(1,"Total Energy Depostion",100,0,100);
    aMan->SetActivation(G4VAnalysisManager::kH1, ih, false);
    ih = aMan->CreateH2(1,"Positional Energy Deposition",100,0,100,100,0,100);
    aMan->SetActivation(G4VAnalysisManager::kH2, ih, false);
}
