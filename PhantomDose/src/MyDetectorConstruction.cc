#include "MyDetectorConstruction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction :: Construct()
{
	// *****************************************************************************
	//								FIND MATERIALS
	// *****************************************************************************
	G4NistManager* nist = G4NistManager::Instance();
	G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");

	// Visualisation attributes of all elements colours 

	G4VisAttributes* colorAir = new G4VisAttributes(G4Colour(0., 0.3, 0.3));
	colorAir->SetVisibility(true);
	colorAir->SetForceSolid(true);
	
	// *****************************************************************************
	//									DEFINE GEOMETRIES
	// *****************************************************************************

	// World volume
	G4Box* solidWorld = new G4Box("solidWorld", 50 * cm, 50 * cm, 150 * cm);
	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
										G4ThreeVector(0., 0., 0.), 
										logicWorld, 
										"physWorld", 
										0, false, 0, true);

	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	// Construct Jaws
	Jaw1X();
	Jaw2X();
	Jaw1Y();
	Jaw2Y();
	MLC();

	//----------------Phantom---------
	phantomDim_x = 15.*cm;
	phantomDim_y = 15.*cm;
	phantomDim_z = 15.*cm;
	G4Box* Phantom = new G4Box("Phantom",phantomDim_x,
									phantomDim_y,phantomDim_z);
	G4Material* Water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	Phantom_log = new G4LogicalVolume(Phantom, Water, "Phantom_log", 0,0,0);
	G4double PhantomPos_x = 0.0*m;
	G4double PhantomPos_y = 0.0*m;
	G4double PhantomPos_z = 115.1561*cm;
	//G4double PhantomPos_z = 115*cm;
	Phantom_phys = new G4PVPlacement(0,
				G4ThreeVector(PhantomPos_x,PhantomPos_y,PhantomPos_z),
				"Phantom_phys",Phantom_log,physWorld,false,0);		

	return physWorld;
}

void MyDetectorConstruction::SetJawAperture(G4int idJaw, G4ThreeVector &centre, G4ThreeVector halfSize, G4RotationMatrix *cRotation)
{
	G4double theta, x, y, z, dx, dy, dz;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();

	dx=halfSize.getX();
	dy=halfSize.getY(); 
	dz=halfSize.getZ();

	// Set field sizes
	G4double isoCentre = 100*cm;
	G4double jaw1XAperture = -5*cm;
	G4double jaw2XAperture = 5*cm;
	G4double jaw1YAperture = -5*cm;
	G4double jaw2YAperture = 5*cm;

	switch (idJaw)
	{
	case 1: //idJaw1XV2100:
		theta = fabs(atan(jaw1XAperture/isoCentre));
		centre.set(z*sin(theta)+dx*cos(theta), y, z*cos(theta)-dx*sin(theta));
		cRotation->rotateY(-theta);
		halfSize.set(fabs(dx*cos(theta)+dz*sin(theta)), fabs(dy), fabs(dz*cos(theta)+dx*sin(theta)));
		break;

	case 2: //idJaw2XV2100:
		theta = fabs(atan(jaw2XAperture/isoCentre));
		centre.set(-(z*sin(theta)+dx*cos(theta)), y, z*cos(theta)-dx*sin(theta));
		cRotation->rotateY(theta);
		halfSize.set(fabs(dx*cos(theta)+dz*sin(theta)), fabs(dy), fabs(dz*cos(theta)+dx*sin(theta)));
		break;

	case 3: //idJaw1YV2100:
		theta = fabs(atan(jaw1YAperture/isoCentre));
		centre.set(x, z*sin(theta)+dy*cos(theta), z*cos(theta)-dy*sin(theta));
		cRotation->rotateX(theta);
		halfSize.set(fabs(dx), fabs(dy*cos(theta)+dz*sin(theta)), fabs(dz*cos(theta)+dy*sin(theta)));
		break;

	case 4: //idJaw2YV2100:
		theta = fabs(atan(jaw2YAperture/isoCentre));
		centre.set(x, -(z*sin(theta)+dy*cos(theta)), z*cos(theta)-dy*sin(theta));
		cRotation->rotateX(-theta);
		halfSize.set(fabs(dx), fabs(dy*cos(theta)+dz*sin(theta)), fabs(dz*cos(theta)+dy*sin(theta)));
		break;
	}
}

void MyDetectorConstruction::Jaw1X()
{
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

	G4String name = "Jaws1X";
	G4Box *box;
	G4LogicalVolume *logVol;
	G4VisAttributes* simpleAlSVisAtt;

	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation = new G4RotationMatrix();

	centre.set(0.,0.,(320.+80./2.)*mm);
	halfSize.set(45.*mm, 93.*mm, 78./2.*mm);
	box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	logVol = new G4LogicalVolume(box, W, name+"LV", 0, 0, 0);
	SetJawAperture(1, centre, halfSize, cRotation);
	new G4PVPlacement(
			cRotation,
			centre,
			logVol,
			name+"PV",
			logicWorld,
			false,
			0,
			check_overlap);


	// Visibility
	simpleAlSVisAtt = new G4VisAttributes(G4Colour::Magenta());
	simpleAlSVisAtt -> SetVisibility(true);
	logVol -> SetVisAttributes(simpleAlSVisAtt);
}

void MyDetectorConstruction::Jaw2X()
{
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

	G4String name = "Jaws2X";
	G4Box *box;
	G4LogicalVolume *logVol;
	G4VisAttributes* simpleAlSVisAtt;

	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();

	centre.set(0.,0.,(320.+80./2.)*mm);
	halfSize.set(45.*mm, 93.*mm, 78./2.*mm);
	box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	logVol = new G4LogicalVolume(box, W, name+"LV", 0, 0, 0);
	SetJawAperture(2, centre, halfSize, cRotation);
	new G4PVPlacement(cRotation, centre, logVol, name+"PV", logicWorld, false, 0, check_overlap);

	// Visibility
	simpleAlSVisAtt = new G4VisAttributes(G4Colour::Magenta());
	simpleAlSVisAtt -> SetVisibility(true);
	logVol->SetVisAttributes(simpleAlSVisAtt);

}

void MyDetectorConstruction::Jaw1Y()
{
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

	G4String name = "Jaws1Y";
	G4Box *box;
	G4LogicalVolume *logVol;
	G4VisAttributes* simpleAlSVisAtt;

	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();

	centre.set(0.,0.,(230.+80./2.)*mm);
	halfSize.set(93.*mm, 35.*mm, 78./2.*mm);
	box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	logVol = new G4LogicalVolume(box, W, name+"LV", 0, 0, 0);
	SetJawAperture(3, centre, halfSize, cRotation);
	new G4PVPlacement(cRotation, centre, logVol, name+"PV", logicWorld, false, 0, check_overlap);

	// Visibility
	simpleAlSVisAtt = new G4VisAttributes(G4Colour::Magenta());
	simpleAlSVisAtt -> SetVisibility(true);
	logVol -> SetVisAttributes(simpleAlSVisAtt);
}

void MyDetectorConstruction::Jaw2Y()
{
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

	G4String name = "Jaws2Y";
	G4Box *box;
	G4LogicalVolume *logVol;
	G4VisAttributes* simpleAlSVisAtt;

	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();

	centre.set(0.,0.,(230.+80./2.)*mm);
	halfSize.set(93.*mm, 35.*mm, 78./2.*mm);
	box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	logVol = new G4LogicalVolume(box, W, name+"LV", 0, 0, 0);
	SetJawAperture(4, centre, halfSize, cRotation);
	new G4PVPlacement(cRotation, centre, logVol, name+"PV", logicWorld, false, 0, check_overlap);

	// Visibility
	simpleAlSVisAtt = new G4VisAttributes(G4Colour::Magenta());
	simpleAlSVisAtt -> SetVisibility(true);
	logVol -> SetVisAttributes(simpleAlSVisAtt);
}

void MyDetectorConstruction::MLC()
{

	G4double leafBPos[120];
	G4double leafAPos[120];

	// Set B leaf pos
	for(int j=0; j<120; j++)
	{
		if(j<=13)
		{
			leafBPos[j] = 0;
		}
		else if (13<j && j<=45)
		{
			leafBPos[j] = 4;
		}
		else
		{
			leafBPos[j] = 0;
		}
		
	}

	// Set A leaf pos
	for(int j=0; j<120; j++)
	{
		if(j<=13)
		{
			leafAPos[j] = 0;
		}
		else if (13<j && j<=45)
		{
			leafAPos[j] = 4;
		}
		else
		{
			leafAPos[j] = 0;
		}
		
	}

	// Construct single leaf B
	G4Material* W = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

	G4Box* solid_thick_Bleaf = new G4Box("solid_thick_Bleaf",16.*cm, 0.25*cm, 3.45*cm);
	G4LogicalVolume* logic_thick_Bleaf = new G4LogicalVolume(solid_thick_Bleaf, W, "logic_thick_Bleaf", 0, 0, 0);
	

	G4int CopyNbBt = 0;

	for(int iB=0; iB<=13; iB++)
	{
		phys_thick_Bleaf = new G4PVPlacement(0, 
						G4ThreeVector(-16.*cm-leafBPos[CopyNbBt]*cm, 10.75*cm-iB*0.5*cm, 46.5*cm),
						logic_thick_Bleaf, "phys_Bleaf", logicWorld, false, CopyNbBt, check_overlap);

		CopyNbBt++;
	}

	G4Box* solid_thin_Bleaf = new G4Box("solid_thick_Bleaf",16.*cm, 0.125*cm, 3.45*cm);
	G4LogicalVolume* logic_thin_Bleaf = new G4LogicalVolume(solid_thin_Bleaf, W, "logic_thin_Bleaf", 0, 0, 0);
	for(int iB=0; iB<=31; iB++)
	{
		phys_thin_Bleaf = new G4PVPlacement(0, 
						G4ThreeVector(-16.*cm-leafBPos[CopyNbBt]*cm, 3.875*cm-iB*0.25*cm, 46.5*cm),
						logic_thin_Bleaf, "phys_Bleaf", logicWorld, false, CopyNbBt, check_overlap);

		CopyNbBt++;
	}

	for(int iB=0; iB<=13; iB++)
	{
		phys_thick_Bleaf = new G4PVPlacement(0, 
						G4ThreeVector(-16.*cm-leafBPos[CopyNbBt]*cm, -4.25*cm-iB*0.5*cm, 46.5*cm),
						logic_thick_Bleaf, "phys_Bleaf", logicWorld, false, CopyNbBt, check_overlap);
		CopyNbBt++;
	}

	// Construct single leaf A

	G4Box* solid_thick_Aleaf = new G4Box("solid_thick_Aleaf",16.*cm, 0.25*cm, 3.45*cm);
	G4LogicalVolume* logic_thick_Aleaf = new G4LogicalVolume(solid_thick_Aleaf, W, "logic_thick_Aleaf", 0, 0, 0);

	G4int CopyNbAt = 0;

	for(int iA=0; iA<=13; iA++)
	{
		phys_thick_Aleaf = new G4PVPlacement(0, 
						G4ThreeVector(16.*cm+leafBPos[CopyNbAt]*cm, 10.75*cm-iA*0.5*cm, 46.5*cm),
						logic_thick_Bleaf, "phys_Aleaf", logicWorld, false, CopyNbAt, check_overlap);

		CopyNbAt++;
	}
	
	G4Box* solid_thin_Aleaf = new G4Box("solid_thin_Aleaf",16.*cm, 0.125*cm, 3.45*cm);
	G4LogicalVolume* logic_thin_Aleaf = new G4LogicalVolume(solid_thin_Aleaf, W, "logic_thin_Aleaf", 0, 0, 0);
	for(int iA=0; iA<=31; iA++)
	{
		phys_thin_Aleaf = new G4PVPlacement(0, 
						G4ThreeVector(16.*cm+leafAPos[CopyNbAt]*cm, 3.875*cm-iA*0.25*cm, 46.5*cm),
						logic_thin_Aleaf, "phys_Aleaf", logicWorld, false, CopyNbAt, check_overlap);

		CopyNbAt++;
	}

	for(int iA=0; iA<=13; iA++)
	{
		phys_thick_Aleaf = new G4PVPlacement(0, 
						G4ThreeVector(16.*cm+leafAPos[CopyNbAt]*cm, -4.25*cm-iA*0.5*cm, 46.5*cm),
						logic_thick_Aleaf, "phys_Aleaf", logicWorld, false, CopyNbAt, check_overlap);
		CopyNbAt++;
	}


}
