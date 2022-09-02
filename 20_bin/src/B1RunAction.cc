//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
  // add new units for dose
  // 

  // Register accumulable to the accumulable manager
  fName = "data";
}

B1RunAction::B1RunAction(const char *name)
: G4UserRunAction()
{
  fName = name;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed

  // reset accumulables to their initial values
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile(fName);

  analysisManager->CreateNtuple("step", "step");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("volumeID");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("event", "energy deposit per event in volume-1");
  analysisManager->CreateNtupleDColumn("edep1");
  analysisManager->FinishNtuple();

  // create histogram
  analysisManager->CreateH1("edep", "edep1", 1000, 0., 0.7*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run*)
{

  // Merge accumulables 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // Compute dose = total energy deposit in a run and its variance
  //

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
        
  // Print
  //  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

