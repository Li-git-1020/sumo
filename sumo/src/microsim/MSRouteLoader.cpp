/****************************************************************************/
/// @file    MSRouteLoader.cpp
/// @author  Daniel Krajzewicz
/// @date    Wed, 6 Nov 2002
/// @version $Id: $
///
// A class that performs the loading of routes
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
// included modules
// ===========================================================================
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <utils/common/FileErrorReporter.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/XMLHelpers.h>
#include "MSNet.h"
#include "MSRouteHandler.h"
#include "MSRouteLoader.h"

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;


// ===========================================================================
// method definitions
// ===========================================================================
MSRouteLoader::MSRouteLoader(MSNet &,
                             MSRouteHandler *handler)
        : myParser(0), _moreAvailable(true), myHandler(handler)
{
    myParser = XMLHelpers::getSAXReader(*myHandler);
}


MSRouteLoader::~MSRouteLoader()
{
    delete myParser;
    delete myHandler;
}


void
MSRouteLoader::init()
{
    _moreAvailable = true;
    if (!myParser->parseFirst(myHandler->getFileName().c_str(), myToken)) {
        MsgHandler::getErrorInstance()->inform("Can not read XML-file '" + myHandler->getFileName() + "'.");
        throw ProcessError();
    }
}


void
MSRouteLoader::loadUntil(SUMOTime time, MSVehicleContainer &into)
{
    // read only when further data is available, no error occured
    //  and vehicles may be found in the between the departure time of
    //  the last read vehicle and the time to read until
    if (!_moreAvailable || time < myHandler->getLastDepart()|| myHandler->errorOccured()) {
        return;
    }

    // if a vehicle was read before the call but was not yet added,
    //  add it now
    MSVehicle *v = myHandler->retrieveLastReadVehicle();
    if (v!=0) {
        into.add(v);
    }
    // read vehicles until specified time or the period to read vehicles
    //  until is reached
    while (myParser->parseNext(myToken)) {
        // return when the last read vehicle is beyond the period
        if (myHandler->getLastDepart()>=time) {
            return;
        }
        // otherwise add the last vehicle read (if any)
        v = myHandler->retrieveLastReadVehicle();
        if (v!=0) {
            into.add(v);
        }
    }

    // no data are available anymore
    _moreAvailable = false;
    return;
}


bool
MSRouteLoader::moreAvailable() const
{
    return _moreAvailable && !myHandler->errorOccured();
}



/****************************************************************************/

