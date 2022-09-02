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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4EventManager.hh"

#include "G4Step.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction()
: G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  G4int volumeID = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
  G4double totalEdep = step->GetTotalEnergyDeposit();

  // pdg encoding
  G4Track* track = step->GetTrack();
  G4int pdg_code = track->GetParticleDefinition()->GetPDGEncoding();
  G4int track_id = track->GetTrackID();
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, 0, eventID);
  analysisManager->FillNtupleIColumn(0, 1, volumeID);
  analysisManager->FillNtupleDColumn(0, 2, totalEdep);
  analysisManager->FillNtupleIColumn(0, 3, pdg_code);
  analysisManager->FillNtupleIColumn(0, 4, track_id);
  analysisManager->AddNtupleRow(0);
  
  // check if we are in scoring volume

  // collect energy deposited in this step
  B1EventAction *eventAction = (B1EventAction *) G4EventManager::GetEventManager()->GetUserEventAction();
  if (volumeID == 1)
    eventAction->AddEnergyDeposit1(totalEdep);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

