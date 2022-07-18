#ifndef MyPrimaryGenerator_h
#define MyPrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Electron.hh"

class G4GeneralParticleSource;
class G4Event;
class PhantomSD;
class G4IAEAphspReader;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	    MyPrimaryGenerator();    
    	virtual ~MyPrimaryGenerator();
public:
    	virtual void GeneratePrimaries(G4Event*);          
private:
        G4IAEAphspReader* theIAEAReader;
};

#endif
