#ifndef PhotonHit_h
#define PhotonHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"

/**
 * @brief - Hit: a snapshot of the physcial interaction of a track in the sensitive region of a detector
 *
 * Contians:
 *  - Positon, arrival time, and kinetic energy
 */
class PhotonHit : public G4VHit {
	public:
		PhotonHit();
		~PhotonHit();


		inline void* operator new(size_t);
		inline void operator delete(void*);

		void Print();

	private:
		G4ThreeVector pos;			            /* Position of the hit                    */
		G4double kEnergy;                   /* Kinetic Energy of the particle         */
    G4double arrivalTime;               /* Global time (time since current event) */

	public:
		void SetPosition(G4ThreeVector p)			{pos = p;};
		void SetKineticEnergy(G4double E)     {kEnergy = E;};
    void SetArrivalTime(G4double t)              {arrivalTime = t;};

		G4ThreeVector GetPosition()			    {return pos;};
    G4double GetArrivalTime()           {return arrivalTime;};
		G4double GetKineticEnergy()         {return kEnergy;};
};

typedef G4THitsCollection<PhotonHit> PhotonHitsCollection;
extern G4Allocator<PhotonHit> PhotonHitAllocator;

inline void* PhotonHit::operator new(size_t){
	void *aHit;
	aHit = (void *) PhotonHitAllocator.MallocSingle();
	return aHit;
}


inline void PhotonHit::operator delete(void *aHit){
	PhotonHitAllocator.FreeSingle((PhotonHit*) aHit);
}
#endif

