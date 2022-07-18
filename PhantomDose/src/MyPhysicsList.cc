#include "MyPhysicsList.hh"

MyPhysicsList::MyPhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics_option3());
}

MyPhysicsList::~MyPhysicsList()
{ 
}

void MyPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
