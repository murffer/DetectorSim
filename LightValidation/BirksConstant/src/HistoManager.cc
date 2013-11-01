#include "HistoManager.hh"
#include "G4UnitsTable.hh"

/**
 * Creates a Histogram manger with the default name
 */
HistoManager::HistoManager(): fFileName("LayeredDetectorCal")
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
  G4AnalysisManager* aMan = G4AnalysisManager::Instance();
  aMan->SetFileName(fFileName);
  aMan->SetVerboseLevel(0);
  aMan->SetFirstHistoId(1);     // start histogram numbering from 1
  aMan->SetActivation(true);    // enable inactivation of histograms


  // Create all histograms as inactivated 
  G4int ih = 0;
  ih = aMan->CreateH1("numPhotonsCreated","Number of Optical Photons Detected",100,0,10000);
  aMan->SetActivation(G4VAnalysisManager::kH1, ih, true);
  ih = aMan->CreateH1("numPhotonsDet","Number of Optical Photons Generated",100,0,10000);
  aMan->SetActivation(G4VAnalysisManager::kH1, ih, true);
}
