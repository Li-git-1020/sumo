/****************************************************************************/
/// @file    MSEventControl.cpp
/// @author  Christian Roessel
/// @date    Mon, 12 Mar 2001
/// @version $Id: $
///
// time-dependant events
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// copyright : (C) 2001-2007
//  by DLR (http://www.dlr.de/) and ZAIK (http://www.zaik.uni-koeln.de/AFS)
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
// ===========================================================================
// compiler pragmas
// ===========================================================================
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif


// ===========================================================================
// included modules
// ===========================================================================
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <cassert>
#include "MSEventControl.h"
#include <utils/common/MsgHandler.h>
#include <utils/helpers/Command.h>
#include "MSNet.h"

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;


// ===========================================================================
// member definitions
// ===========================================================================
/* -------------------------------------------------------------------------
 * methods from MSEventControl
 * ----------------------------------------------------------------------- */
MSEventControl::MSEventControl() :
        myEvents()
{}


MSEventControl::~MSEventControl()
{
    // Empty the event-container and delete the commands.
    while (! myEvents.empty()) {
        Event e = myEvents.top();
        delete e.first;
        myEvents.pop();
    }
}


SUMOTime
MSEventControl::addEvent(Command* operation,
                         SUMOTime execTimeStep,
                         AdaptType type)
{
    SUMOTime currTimeStep = MSNet::getInstance()->getCurrentTimeStep();
    if (type == ADAPT_AFTER_EXECUTION && execTimeStep <= currTimeStep) {
        execTimeStep = currTimeStep;
    }
    Event newEvent = Event(operation, execTimeStep);
    myEvents.push(newEvent);
    return execTimeStep;
}


void
MSEventControl::execute(SUMOTime execTime)
{
    // Execute all events that are scheduled for execTime.
    for (;!myEvents.empty();) {

        Event currEvent = myEvents.top();

        if (currEvent.second == execTime) {

            Command *command = currEvent.first;
            myEvents.pop();
            SUMOTime time = command->execute(execTime);

            // Delete nonrecurring events, reinsert recurring ones
            // with new execution time = execTime + returned offset.
            if (time == 0) {
                delete currEvent.first;
            } else {
                assert(time > 0);
                currEvent.second = execTime + time;
                myEvents.push(currEvent);
            }
        } else {
            if (currEvent.second < execTime) {
                // !!! more verbose information
                WRITE_WARNING("Could not execute scheduled event.");
                delete currEvent.first;
                myEvents.pop();
            } else {
                break;
            }
        }
    }
}



/****************************************************************************/

