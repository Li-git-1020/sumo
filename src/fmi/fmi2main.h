/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2020-2020 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    fmi2main.h
/// @author  Robert Hilbrich
/// @date    Mon, 24 Aug 2020
///
//  Basic declarations for the FMI model
/****************************************************************************/

#ifndef FMI2MAIN_H
#define FMI2MAIN_H

#include <stdbool.h>

/* Type definitions for callback functions */
typedef void* (*allocateMemoryType)(size_t nobj, size_t size);
typedef void  (*loggerType)        (void *componentEnvironment, const char *instanceName, int status, const char *category, const char *message, ...);
typedef void  (*freeMemoryType)    (void *obj);


/* The core date (= variables of the model) */
typedef struct {
    double vehicleCount;
} ModelData;

/* Several declarations for the model component (housekeeping stuff) */
typedef struct {
    void *componentEnvironment;
    const char *instanceName;
    const char *resourceLocation;

    ModelData *modelData;

    loggerType logger;
   	allocateMemoryType allocateMemory;
   	freeMemoryType freeMemory;

    bool logEvents;
    bool logErrors;
} ModelInstance;

#endif /* FMI2MAIN_H */