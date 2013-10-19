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
    // Create or get analysis manager
    G4AnalysisManager* aMan = G4AnalysisManager::Instance();
    aMan->SetFileName(fFileName);
    aMan->SetVerboseLevel(0);
    aMan->SetFirstHistoId(1);     // start histogram numbering from 1
    aMan->SetActivation(true);    // enable inactivation of histograms
    
    
    // Define histograms start values
    const G4int kMaxHisto = 3;
    const G4String id[] = { "1","2","3"};
    const G4String title[] =
    { "Energy Depostion",                       // 1
        "Number of Optical Photons Created",    // 2
        "Number of Optical Phtons Detected"     // 3
    };
    
    // Default values (to be reset via /analysis/h1/set command)
    G4int nbins = 100;
    G4double vmin = 0.;
    G4double vmax = 100.;
    
    // Create all histograms as inactivated
    for (G4int k=0; k<kMaxHisto; k++) {
        G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
        analysisManager->SetActivation(G4VAnalysisManager::kH1, ih, false);
    }
}
