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
// $Id: WLSRunAction.cc 70603 2013-06-03 11:23:16Z gcosmo $
//
/// \file optical/wls/src/WLSRunAction.cc
/// \brief Implementation of the WLSRunAction class
//
//
#include "WLSRunAction.hh"
#include "WLSRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "Randomize.hh"

#include "WLSDetectorConstruction.hh"
#include "WLSSteppingAction.hh"

#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSRunAction::WLSRunAction(G4String name)
  : fSaveRndm(0), fAutoSeed(false), fName(name)
{
  fRunMessenger = new WLSRunActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSRunAction::~WLSRunAction()
{
  delete fRunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLSRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

  G4AnalysisManager* ana = G4AnalysisManager::Instance();
  G4cout << "### G4AnalysisManager : " << ana->GetType() << G4endl;
	// Open an output file
	ana->OpenFile(fName);
	// Create ntuple
	//ana->SetVerboseLevel(1);
	ana->CreateNtuple("cube", "n, e, x, y, z, nphotons, npx, npy, npz, time, lasttime");
	ana->CreateNtupleDColumn("n"); 
	ana->CreateNtupleDColumn("e"); 
	ana->CreateNtupleDColumn("x"); 
	ana->CreateNtupleDColumn("y"); 
	ana->CreateNtupleDColumn("z"); 
	ana->CreateNtupleDColumn("nPhotons"); 
	ana->CreateNtupleDColumn("npx"); 
	ana->CreateNtupleDColumn("npy"); 
	ana->CreateNtupleDColumn("npz"); 
	ana->CreateNtupleDColumn("time");
	ana->CreateNtupleDColumn("lasttime");
	ana->FinishNtuple(0);

  if (fAutoSeed) {
     // automatic (time-based) random seeds for each run
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     time_t systime = time(NULL);
     seeds[0] = (long) systime;
     seeds[1] = (long) (systime*G4UniformRand());
     G4Random::setTheSeeds(seeds);
     G4Random::showEngineStatus();
  } else {
     G4Random::showEngineStatus();
  }

  if (fSaveRndm > 0) G4Random::saveEngineStatus("BeginOfRun.rndm");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLSRunAction::EndOfRunAction(const G4Run* )
{
  if (fSaveRndm == 1)
  {
     G4Random::showEngineStatus();
     G4Random::saveEngineStatus("endOfRun.rndm");
  }

	G4AnalysisManager* ana = G4AnalysisManager::Instance(); 
	ana->Write();
	ana->CloseFile();
}
