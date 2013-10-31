#include "EventAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

EventAction::EventAction() : G4UserEventAction(){ }

/*
 * BeginOfEventAction
 *
 * Called before the begining of each event
 */
void EventAction::BeginOfEventAction(const G4Event* event){
    Analysis::GetInstance()->PrepareNewEvent(event);
}

/**
 * EndOfEventAction
 *
 * @param const G4Event* event - event to be processed
 *
 * At the end of an event we want to call analysis to proccess
 * this event, and record the useful information.
 */
void EventAction::EndOfEventAction(const G4Event* event){  
    Analysis::GetInstance()->EndOfEvent(event);
}
