#ifndef CaloHit_h
#define CaloHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"

/**
 * @brief - CaloHit: a snapshot of the physcial interaction of a track in the sensitive region of a detector
 *
 * Contians:
 *  - Particle Information (type and rank (primary, secondary, tertiary ...))
 *  - Positon and time
 *  - momentum and kinetic energy
 *  -  deposition in volume
 *  - geometric information
 */
class CaloHit : public G4VHit {
	public:
		CaloHit();
		~CaloHit();


		inline void* operator new(size_t);
		inline void operator delete(void*);

		void Print();

	private:
		G4double edep;				        /* Energy Deposited at the Hit */
		G4ThreeVector pos;			        /* Position of the hit */
		G4double stepLength;		        /* Step Length */
		G4ThreeVector momentum;		        /* Momentrum of the step */
		G4double kEnergy;                   /* Kinetic Energy of the particle */
		G4int trackID;				        /* Track ID */
		G4int parentID;                     /* Parent ID */
    G4ParticleDefinition* particle;     /* Particle Definition */
		G4int particleRank;                 /* Primary, Secondary, etc */
    G4int chamberNumber;            /* Chamber number / copy number */
		G4VPhysicalVolume* volume;			/* Physical Volume */

	public:
		void SetTrackID(G4int track)				{trackID = track;};
		void SetParentID(G4int id)				    {parentID = id;};
		void SetEdep(G4double de)					{edep = de;};
		void SetPosition(G4ThreeVector p)			{pos = p;};
		void SetStepLength(G4double dl)				{stepLength = dl;};
		void SetMomentum(G4ThreeVector p)			{momentum = p;};
		void SetKineticEnergy(G4double E)           {kEnergy = E;};
		void SetParticle(G4ParticleDefinition* pdef)
		{particle = pdef;};
		void SetParticleRank(G4int rank)            {particleRank = rank;};
		void SetVolume(G4VPhysicalVolume* v)	    {volume = v;};
    void SetChamberNb(G4int nb)           {chamberNumber = nb;};

		G4int GetTrackID()					{return trackID;};
    G4int GetParentID()                 {return parentID;};
		G4ThreeVector GetPosition()			{return pos;};
		G4ThreeVector GetMomentum()			{return momentum;};
		G4double GetKineticEnergy()         {return kEnergy;};
		G4double GetEdep()					{return edep;};
		G4double GetStepLength()			{return stepLength;};
		G4ParticleDefinition* GetParticle() {return particle;};
		G4int GetParticleRank()             {return particleRank;};
    G4int GetChamberNumber()            {return chamberNumber;};
		G4VPhysicalVolume* GetVolume()      {return volume;};
};

typedef G4THitsCollection<CaloHit> HitsCollection;
extern G4Allocator<CaloHit> HitAllocator;

inline void* CaloHit::operator new(size_t){
	void *aHit;
	aHit = (void *) HitAllocator.MallocSingle();
	return aHit;
}


inline void CaloHit::operator delete(void *aHit){
	HitAllocator.FreeSingle((CaloHit*) aHit);
}
#endif

