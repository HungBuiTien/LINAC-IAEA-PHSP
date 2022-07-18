#include <iostream>
#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyActionInitialization.hh"

#include "G4ScoringManager.hh"
#include <ctime>

int main(int argc, char** argv)
{	
	G4int seconds = time(NULL);
	G4Random::setTheSeed(seconds);

	//G4RunManager *runManager = new G4RunManager();

	// Construct the default run manager
	auto* runManager = G4RunManagerFactory::CreateRunManager();
	G4int nThreads = 4;
	runManager->SetNumberOfThreads(nThreads);

	// Activate UI-command base scorer
	G4ScoringManager* scManager = G4ScoringManager::GetScoringManager();
	scManager->SetVerboseLevel(1);

	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new MyActionInitialization());
	runManager->Initialize();
		
	
	G4UIExecutive* ui = 0;
	if (argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}
	
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();
	
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	if (ui)
	{
		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
	}
	else
	{
		G4String command = "/control/execute";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}

	return 0;
}
