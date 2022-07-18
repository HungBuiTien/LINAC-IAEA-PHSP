// This is the program to simulate Gamma ART-6000TM
// Version: 2.0
// Date: 02/09/2022
// Author: Hung Bui Tien
// Email: hungbuitien19081997@gmail.com
// Organization: Hanoi University of Science and Technology 


#include <iostream>

#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"

#include "G4Types.hh"
#include "Randomize.hh"
#include <ctime>

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"


using namespace std;

int main(int argc, char** argv)
{
	G4Random::setTheEngine(new CLHEP::RanecuEngine);

	// Construct the default run manager
	//auto* runManager = G4RunManagerFactory::CreateRunManager();
	//G4int nThreads = 4;
	//runManager->SetNumberOfThreads(nThreads);
	G4RunManager* runManager = new G4RunManager;
	

	// Detect interactive mode (if no macro provided) and define UI session
    //
	G4UIExecutive* ui = 0;

	// Set mandatory initialization classes
	//
	runManager->SetUserInitialization(new DetectorConstruction());
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new ActionInitialization()); 


	// Initialize G4 kernel
	//
	runManager->Initialize();

	// Process macro or start UI session
	//
	if (argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}

	// Initialize visualization
	//
	G4VisManager* visManager = new G4VisExecutive();
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (ui)
	{
		// interactive mode : define UI session
		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
	}
	else
	{
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	
	delete visManager;
	delete runManager;

	return 0;
}