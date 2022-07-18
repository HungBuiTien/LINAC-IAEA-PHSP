#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorMessenger.hh"
#include "G4Threading.hh"
#include "G4SDManager.hh"
#include "G4AutoDelete.hh"
#include"G4RegionStore.hh"
#include"globals.hh"
#include"G4Material.hh"
#include"G4MaterialTable.hh"
#include"G4Element.hh"
#include"G4ElementTable.hh"
#include"G4Box.hh"
#include"G4Region.hh"
#include"G4ThreeVector.hh"
#include"G4PVPlacement.hh"
#include"G4PVReplica.hh"
#include"G4SDManager.hh"
#include"G4VisAttributes.hh"
#include"G4Colour.hh"
#include"G4Tubs.hh"
#include"G4Cons.hh"
#include"G4Paraboloid.hh"
#include"G4SubtractionSolid.hh"
#include"G4UnionSolid.hh"
#include"G4IntersectionSolid.hh"
#include "G4tgbVolumeMgr.hh"
#include "G4tgrMessenger.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
 pDetectorMessenger= new DetectorMessenger(this);
}


DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  // Copper
  G4Material* Cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4VisAttributes* CopperVisAtt;
	CopperVisAtt = new G4VisAttributes(G4Colour::Magenta());
	CopperVisAtt -> SetVisibility(true);

  // Tungsten
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  G4VisAttributes *TungstenVisAtt;
  TungstenVisAtt = new G4VisAttributes(G4Colour::Cyan());
	TungstenVisAtt -> SetVisibility(true);

  // Berilium
  G4Material *Be = G4NistManager::Instance()->FindOrBuildMaterial("G4_Be");
  G4VisAttributes* BeriliumSVisAtt = new G4VisAttributes(G4Colour::Yellow());
	BeriliumSVisAtt -> SetVisibility(true);

  // Kapton
  G4Material *material=G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
  G4VisAttributes* KaptonWVisAtt;
  KaptonWVisAtt= new G4VisAttributes(G4Colour::Blue());
	KaptonWVisAtt->SetVisibility(true);

  G4VisAttributes* KaptonPVisAtt;
  KaptonPVisAtt= new G4VisAttributes(G4Colour::Yellow());
	KaptonPVisAtt->SetVisibility(true);

  // Mylar
  G4Material *MYLAR = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
	G4VisAttributes* MylarVisAtt;
  MylarVisAtt = new G4VisAttributes(G4Colour::Green());
	MylarVisAtt -> SetVisibility(true);

  // ******************************************************************************************************
	//									World volume
	// ******************************************************************************************************

  G4bool check_overlap = true;
	// World volume
	G4Box* solidWorld = new G4Box("solidWorld", 30 * cm, 30 * cm, 50 * cm);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "logicWorld");
	physWorld = new G4PVPlacement(0,
										G4ThreeVector(0., 0., 0.), 
										logicWorld, 
										"physWorld", 
										0, false, 0, true);
  //logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  // ******************************************************************************************************
	//									Target volume
	// ******************************************************************************************************

	G4Box* targetA_box = new G4Box("targetA_box",0.6*cm,0.6*cm,0.04445*cm);
	G4LogicalVolume *targetA_log = new G4LogicalVolume(targetA_box,W,"targetA_log",0,0,0);
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.20055*cm), targetA_log, "targetA", logicWorld, false, 0, check_overlap);
	targetA_log -> SetVisAttributes(TungstenVisAtt);
	
	G4Box* targetB_box = new G4Box("targetB_box", 0.6*cm, 0.6*cm, 0.07874*cm);
	G4LogicalVolume *targetB_log = new G4LogicalVolume(targetB_box,Cu,"targetB_log",0,0,0);
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.07736*cm), targetB_log, "targetB", logicWorld, false, 0, check_overlap);
	targetB_log -> SetVisAttributes(CopperVisAtt);

  // ******************************************************************************************************
	//									Primary collimator volume
	// ******************************************************************************************************

	//---------rotation matrix first collimator --------
	G4RotationMatrix*  rotateMatrix=new G4RotationMatrix();
	rotateMatrix->rotateX(180.0*deg);

	//--------------------upper collimator----------------
	G4double innerRadiusOfTheTubeEx = 1.0*cm;
	G4double outerRadiusOfTheTubeEx = 8.*cm;
	G4double hightOfTheTubeEx = 3.0*cm;
	G4double startAngleOfTheTubeEx = 0.*deg;
	G4double spanningAngleOfTheTubeEx = 360.*deg;
	G4Tubs* UpperCollimator = new G4Tubs("UpperCollimator",
			innerRadiusOfTheTubeEx,
			outerRadiusOfTheTubeEx,
			hightOfTheTubeEx,
			startAngleOfTheTubeEx,
			spanningAngleOfTheTubeEx);
	G4LogicalVolume *UpperCollimator_log = new G4LogicalVolume(UpperCollimator, W, "UpperCollimator_log", 0, 0, 0);

	G4double UpperCollimatorPosX = 0.*cm;
	G4double UpperCollimatorPosY = 0.*cm;
	G4double UpperCollimatorPosZ = -1.*cm;
	new G4PVPlacement(0,
			G4ThreeVector(UpperCollimatorPosX, UpperCollimatorPosY, UpperCollimatorPosZ),
			UpperCollimator_log,
			"UpperCollimator",
			logicWorld,
			false,
			0,
			check_overlap);

	//--------------------lower collimator----------------

	G4double  pRmin1 = 0.*cm;
	G4double  pRmax1 = 0.5*cm;
	G4double  pRmin2 = 0.*cm;
	G4double  pRmax2 = 1.7658592*cm;
	G4double  hightOfTheCone =3.2*cm;
	G4double  startAngleOfTheCone = 0.*deg;
	G4double  spanningAngleOfTheCone = 360.*deg;

	G4Cons* collim_cone = new G4Cons("collim_cone",pRmin1,pRmax1,pRmin2,
		  pRmax2,hightOfTheCone,startAngleOfTheCone,
		  spanningAngleOfTheCone);
	G4LogicalVolume *collim_log = new G4LogicalVolume(collim_cone,Vacuum,"collim_log",0,0,0);

	G4double innerRadiusOfTheTube = 0.*cm;
	G4double outerRadiusOfTheTube = 8.*cm;
	G4double hightOfTheTube = 3.1*cm;
	G4double startAngleOfTheTube = 0.*deg;
	G4double spanningAngleOfTheTube = 360.*deg;
	G4Tubs* tracker_tube = new G4Tubs("tracker_tube",innerRadiusOfTheTube,
			outerRadiusOfTheTube,hightOfTheTube,
			startAngleOfTheTube,spanningAngleOfTheTube);

	G4SubtractionSolid* CylMinusCone = new G4SubtractionSolid("Cyl-Cone",
			tracker_tube,collim_cone);
	G4LogicalVolume *CylMinusCone_log = new G4LogicalVolume(CylMinusCone,W,"CylminusCone_log",0,0,0);
	G4double CminusCPos_x = 0.*cm;
	G4double CminusCPos_y = 0.*cm;
	G4double CminusCPos_z = +6.2*cm;
	new G4PVPlacement(rotateMatrix,
			G4ThreeVector(CminusCPos_x, CminusCPos_y, CminusCPos_z),
			CylMinusCone_log,
			"CylMinusCone",
			logicWorld,
			false,
			0,
			check_overlap);

  CylMinusCone_log->SetVisAttributes(TungstenVisAtt);
	UpperCollimator_log->SetVisAttributes(TungstenVisAtt);

  	
  // ******************************************************************************************************
	//									Window volume
	// ******************************************************************************************************

	G4Tubs* BeWTube = new G4Tubs("BeWindowTube", 0., 36.*mm, 0.2*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *BeWTubeLV = new G4LogicalVolume(BeWTube, Be, "BeWTubeLV", 0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0.,0.,100.*mm), BeWTubeLV, "BeWTubePV",
			logicWorld, false, 0, check_overlap);
  BeWTubeLV -> SetVisAttributes(BeriliumSVisAtt);


  // ******************************************************************************************************
	//									Monitor volume
	// ******************************************************************************************************

	G4Tubs* ICTubeW = new G4Tubs("ionizationChamberTube", 0., 2.*2.54*10.*mm, 0.016*25.4*mm, 0.*deg, 360.*deg);
	G4Tubs* ICTubeP = new G4Tubs("ionizationChamberTube", 0., 2.*2.54*10.*mm, 0.010*25.4*mm, 0.*deg, 360.*deg);

	G4ThreeVector centre;
	// W1
	centre.set(0.,0.,157.*mm);
	G4LogicalVolume *PCUTubeW1LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeW1LV, "ionizationChamberTubeW1PV", logicWorld, false, 0, check_overlap);
  PCUTubeW1LV->SetVisAttributes(KaptonWVisAtt);

	// P1
	centre.set(0.,0.,158.*mm);
	G4LogicalVolume *PCUTubeP1LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeP1LV, "ionizationChamberTubeP1PV", logicWorld, false, 0, check_overlap);
	PCUTubeP1LV->SetVisAttributes(KaptonPVisAtt);

	// W2
	centre.set(0.,0.,159.*mm);
	G4LogicalVolume *PCUTubeW2LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeW2LV, "ionizationChamberTubeW2PV", logicWorld, false, 0, check_overlap);
	PCUTubeW2LV->SetVisAttributes(KaptonWVisAtt);

	// P2
	centre.set(0.,0.,160.*mm);
	G4LogicalVolume *PCUTubeP2LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeP2LV, "ionizationChamberTubeP2PV", logicWorld, false, 0, check_overlap);
	PCUTubeP2LV->SetVisAttributes(KaptonPVisAtt);

	// W3
	centre.set(0.,0.,161.*mm);
	G4LogicalVolume *PCUTubeW3LV = new G4LogicalVolume(ICTubeW, material, "ionizationChamberTubeW3LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeW3LV, "ionizationChamberTubeW3PV", logicWorld, false, 0, check_overlap);
	PCUTubeW3LV->SetVisAttributes(KaptonWVisAtt);

	// P3
	centre.set(0.,0.,162.*mm);
	G4LogicalVolume *PCUTubeP3LV = new G4LogicalVolume(ICTubeP, material, "ionizationChamberTubeP3LV", 0, 0, 0);
	new G4PVPlacement(0, centre, PCUTubeP3LV, "ionizationChamberTubeP3PV", logicWorld, false, 0, check_overlap);
	PCUTubeP3LV->SetVisAttributes(KaptonPVisAtt);

  // ******************************************************************************************************
	//									Mirror volume
	// ******************************************************************************************************

	G4Tubs* MirrorTube = new G4Tubs("MirrorTube", 0., 63.*mm, .5*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *MirrorTubeLV = new G4LogicalVolume(MirrorTube, MYLAR, "MirrorTubeLV", 0, 0, 0);
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	cRotation -> rotateY(12.0*deg);
	new G4PVPlacement(cRotation, G4ThreeVector(0., 0., 175.*mm), MirrorTubeLV, "MirrorTubePV",logicWorld, false, 0, check_overlap);
	MirrorTubeLV -> SetVisAttributes(MylarVisAtt);

  // Always return Physic World volume
  return physWorld;
}


void DetectorConstruction::ConstructSDandField()
{}
