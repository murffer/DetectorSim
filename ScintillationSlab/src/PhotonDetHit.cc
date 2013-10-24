#include "PhotonDetHit.hh"

G4Allocator<PhotonDetHit> PhotonDetHitAllocator;

PhotonDetHit::PhotonDetHit()
{
  arrivalTime = 0.;
}

PhotonDetHit::PhotonDetHit(G4double pTime)
{
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
  arrivalTime = right.arrivalTime;

  return *this;
}

G4int PhotonDetHit::operator==(const PhotonDetHit& right) const
{
  return  arrivalTime == right.arrivalTime;  
}

void PhotonDetHit::Draw(){ }

void PhotonDetHit::Print(){ }
