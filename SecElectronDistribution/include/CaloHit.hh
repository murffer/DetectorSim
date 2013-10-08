#ifndef CaloHit_h
#define CaloHit_h 1

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
		G4VPhysicalVolume* volume;			/* Physical Volume */
    G4String creatorName;          /* Process that created this track */
    G4String postProcessName;       /* Post process name */ 
    bool firstStep;                 /* First step in the volume */
public:
		void SetTrackID(G4int track)				{trackID = track;};
		void SetParentID(G4int id)				    {parentID = id;};
		void SetEdep(G4double de)					{edep = de;};
		void SetPosition(G4ThreeVector p)			{pos = p;};
		void SetStepLength(G4double dl)				{stepLength = dl;};
		void SetMomentum(G4ThreeVector p)			{momentum = p;};
		void SetKineticEnergy(G4double E)           {kEnergy = E;};
		void SetParticle(G4ParticleDefinition* pdef){particle = pdef;};
		void SetVolume(G4VPhysicalVolume* v)	    {volume = v;};
    void SetCreatorProcess(G4String p) {creatorName = p;};
    void SetPostProcess(G4String p)    {postProcessName = p;};
    void SetFirstStep(bool f)          {firstStep = f;};

		G4int GetTrackID()					{return trackID;};
    G4int GetParentID()                 {return parentID;};
		G4ThreeVector GetPosition()			{return pos;};
		G4ThreeVector GetMomentum()			{return momentum;};
		G4double GetKineticEnergy()         {return kEnergy;};
		G4double GetEdep()					{return edep;};
		G4double GetStepLength()			{return stepLength;};
		G4ParticleDefinition* GetParticle() {return particle;};
		G4VPhysicalVolume* GetVolume()      {return volume;};
    G4String GetCreatorProcess() {return creatorName;};
    G4String GetPostProcessName() {return postProcessName;};
    bool GetFirstStep() {return firstStep;};
};


typedef G4THitsCollection<CaloHit> CaloHitsCollection;
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

