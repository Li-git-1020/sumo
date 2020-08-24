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
/// @file    fmi2main.c
/// @author  Robert Hilbrich
/// @date    Tue, 03 Mar 2020
///
// Implementation of the FMI2 to sumolib bridge features
/****************************************************************************/

#define FMI_VERSION 2
#include <foreign/fmi/fmi2Functions.h>

#include "libsumocpp2c.h"
#include "fmi2main.h"

void
fmi2run() {
    libsumo_run();
}
