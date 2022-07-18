#include "MyPrimaryGenerator.hh"

#include "G4Electron.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "G4IAEAphspReader.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    G4String fileName="VARIAN-S-BREMSPE";

    theIAEAReader = new G4IAEAphspReader(fileName);
    // phase-space plane shift
    // G4ThreeVector psfShift(0., 0.,-50*cm);//lespace de phase est 
    // theIAEAReader->SetGlobalPhspTranslation(psfShift);
    // theIAEAReader->SetTimesRecycled(0);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
  	if (theIAEAReader) delete theIAEAReader;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    theIAEAReader->GeneratePrimaryVertex(anEvent);
}

