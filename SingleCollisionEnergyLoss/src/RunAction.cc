/**
 * @file    RunAction.hh
 *
 * @date    5 July 2012
 * @author  murffer
 *
 * @brief   Implements user class RunAction
 */
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "Analysis.hh"

RunAction::RunAction() : G4UserRunAction()
{ 
}


void RunAction::BeginOfRunAction(const G4Run* run)
{ 
	G4cout<<"Starting  run: " << run->GetRunID()<< G4endl;
    Analysis::GetInstance()->PrepareNewRun(run);
}


void RunAction::EndOfRunAction(const G4Run* aRun)
{
    Analysis::GetInstance()->EndOfRun(aRun);
}
