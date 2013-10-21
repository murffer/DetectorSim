/**
 * @file    EventAction.hh
 *
 * @date    5 July 2012
 * @author  murffer@utk.edu
 *
 * @brief   User's EventAction
 */
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Analysis.hh"

class G4Event;

/*!
 * \brief User's EventAction class
 * This class implements the Event Action.
 *
 */


class EventAction : public G4UserEventAction{
    public:
        EventAction();
        virtual ~EventAction() {};
        //! Beginning of the event
        virtual void  BeginOfEventAction(const G4Event* event);
        //! Digitize hits and store information
        virtual void    EndOfEventAction(const G4Event* event);

    private:
};
#endif
