#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

#include "g4root.hh"
//#include "g4xml.hh"


class HistoManager
{
  public:
   HistoManager();
  ~HistoManager();

  private:
    void Book();
    G4String fFileName;   /** Default filename */
};
#endif

