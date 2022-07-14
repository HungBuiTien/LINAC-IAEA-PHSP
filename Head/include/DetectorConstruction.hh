#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GenericPolycone.hh"
#include"G4Tubs.hh"
#include"G4Cons.hh"
#include"G4Paraboloid.hh"
#include"G4SubtractionSolid.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4Box;
class DetectorMessenger;
class G4tgrMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  //Messengers
  DetectorMessenger * pDetectorMessenger;

  // Physical Detectors
  G4VPhysicalVolume * physWorld;

};
#endif