/**
 * @file      cores_controller.cpp
 * @author    Bruno de Carvalho Albertini
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
 * @brief     Implements a ac_tlm cores_controller.
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

#include "cores_controller.h"

//////////////////////////////////////////////////////////////////////////////


/// Constructor
ac_tlm_mem::ac_tlm_mem( sc_module_name module_name , int k ) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the cores_controller
    target_export( *this );

    /// Initialize cores_controller vector
    cores_controller = new uint8_t[ k ];
    for(k=k-1;k>0;k--) cores_controller[k]=0;

}

/// Destructor
ac_tlm_mem::~ac_tlm_mem() {

  delete [] cores_controller;
}

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_mem::writem( const uint32_t &a , const uint32_t &d )
{
  *((uint32_t *) &cores_controller[a]) = *((uint32_t *) &d);
  return SUCCESS;
}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_mem::readm( const uint32_t &a , uint32_t &d )
{
  *((uint32_t *) &d) = *((uint32_t *) &cores_controller[a]);

  return SUCCESS;
}

bool ac_tlm_mem::set_core(bool on, int core_num) {
  if (cores_on(core_num) == on) {
    return false;
  }
  if (on) {
    cores->at(core_num).init();
  } else {
    cores->at(core_num).stop();
  }
  return true;
}

int ac_tlm_mem::number_of_cores() {
  return cores->size();
}

bool ac_tlm_mem::cores_on(int core_num) {
  return cores_on_.at(core_num);
}


