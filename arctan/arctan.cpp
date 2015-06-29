/**
 * @file      arctan.cpp
 * @author    Gabriel Borges
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 *
 * @brief     Implements a ac_tlm arctan.
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "arctan.h"

//////////////////////////////////////////////////////////////////////////////


/// Constructor
ac_arctan::ac_arctan( sc_module_name module_name ) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the arctan
    target_export( *this );
}

void setThreadID(int core, int threadID){

}

void setIntervals(int core, int intervals){

}

void setNumThreads(int core, int numThreads){

}

double getResult(int core){

}
