#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4MultiUnion.hh"

class MedLinacPhantomSD;
class MedLinacPhantomROGeometry;

class MyDetectorConstruction : public G4VUserDetectorConstruction
{

public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();
	void ConstructSensitiveDetector();

public:
	G4bool check_overlap = true;
	G4LogicalVolume *logicWorld;
	G4VPhysicalVolume *phys_thick_Bleaf, *phys_thin_Bleaf, *phys_thick_Aleaf, *phys_thin_Aleaf;
	G4LogicalVolume* Phantom_log;
	G4VPhysicalVolume* Phantom_phys;

	MedLinacPhantomSD* phantomSD;//pointer to sensitive detector
  	MedLinacPhantomROGeometry* phantomROGeometry;//pointer to ROGeometry

	G4double phantomDim_x;  //Phantom XDimension
	G4double phantomDim_y; //Phantom YDimension
	G4double phantomDim_z; //Phantom ZDimension 

	G4int numberOfVoxelsAlongX; //Number of voxels along x axis
	G4int numberOfVoxelsAlongY; //Number of voxels along y axis
	G4int numberOfVoxelsAlongZ; //Number of voxels along z axis

	G4String sensitiveDetectorName; 

private:
	void SetJawAperture(G4int idJaw, G4ThreeVector &centre, G4ThreeVector halfSize, G4RotationMatrix *cRotation);
	void Jaw1X();
	void Jaw2X();
	void Jaw1Y();
	void Jaw2Y();
	void WaterPhantom();
	void MLC();
};

#endif
