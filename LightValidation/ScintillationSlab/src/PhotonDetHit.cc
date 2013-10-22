#include "PhotonDetHit.hh"

G4Allocator<PhotonDetHit> PhotonDetHitAllocator;

PhotonDetHit::PhotonDetHit()
{
  posExit     = G4ThreeVector(0., 0., 0.);
  posArrive   = G4ThreeVector(0., 0., 0.);
  arrivalTime = 0.;
}

PhotonDetHit::PhotonDetHit(G4ThreeVector pExit,
                                 G4ThreeVector pArrive,
                                 G4double pTime)
{
  posExit     = pExit;
  posArrive   = pArrive;
  arrivalTime = pTime;
}

PhotonDetHit::~PhotonDetHit() { }

PhotonDetHit::PhotonDetHit(const PhotonDetHit &right)
  : G4VHit()
{
  *this = right;
}

const PhotonDetHit& PhotonDetHit::operator=(const PhotonDetHit &right)
{
  posExit     = right.posExit;
  posArrive   = right.posArrive;
  arrivalTime = right.arrivalTime;

  return *this;
}

G4int PhotonDetHit::operator==(const PhotonDetHit& right) const
{
  return posExit     == right.posExit    &&
         posArrive   == right.posArrive  &&
         arrivalTime == right.arrivalTime;  
}

void PhotonDetHit::Draw(){ }

void PhotonDetHit::Print(){ }
