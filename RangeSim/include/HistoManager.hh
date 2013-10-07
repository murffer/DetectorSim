
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

#include "g4root.hh"
//#include "g4xml.hh"
////#include "g4hbook.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:
   HistoManager();
  ~HistoManager();

  private:
    void Book();
    G4String fFileName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

