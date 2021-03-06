#ifndef PhotonDetHit_h
#define PhotonDetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

//--------------------------------------------------
// PhotonDetHit Class
//--------------------------------------------------

class PhotonDetHit : public G4VHit
{
  public:

    PhotonDetHit();
    PhotonDetHit(G4double pTime);
    ~PhotonDetHit();

    PhotonDetHit(const PhotonDetHit &right);
    const PhotonDetHit& operator=(const PhotonDetHit& right);

    G4int operator==(const PhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw();
    void Print();

    inline void SetArrivalTime(G4double t) { arrivalTime = t; }
    inline G4double GetArrivalTime() { return arrivalTime; }
 
  private:

    // the arrival time of the photon
    G4double      arrivalTime;

};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<PhotonDetHit> PhotonDetHitsCollection;

extern G4Allocator<PhotonDetHit> PhotonDetHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* PhotonDetHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) PhotonDetHitAllocator.MallocSingle();
  return aHit;
}

inline void PhotonDetHit::operator delete(void *aHit)
{
  PhotonDetHitAllocator.FreeSingle((PhotonDetHit*) aHit);
}

#endif
