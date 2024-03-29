/**
 * @file      cores_controller.h
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
 * @brief     Defines a ac_tlm cores_controller.
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

#ifndef _cores_controller_H_
#define _cores_controller_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
#include <vector>
// SystemC includes
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include "../mips/mips.H"

//////////////////////////////////////////////////////////////////////////////

// using statements
using tlm::tlm_transport_if;

//////////////////////////////////////////////////////////////////////////////

// #define DEBUG

#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242984
#define ADDRESS_CORE_ZERO_WRITE 115243080


/// A TLM cores_controller
class ac_cores_controller :
  public sc_module,
  public ac_tlm_transport_if // Using ArchC TLM protocol
{
public:
  bool set_core(bool on, int core);
  int number_of_cores();
  bool is_core_on(int core_num);

  /// Exposed port with ArchC interface
  sc_export< ac_tlm_transport_if > target_export;

  /**
   * Implementation of TLM transport method that
   * handles packets of the protocol doing appropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * @param request is a received request packet
   * @return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request ) {

    ac_tlm_rsp response;

    switch( request.type ) {
    case READ :     // Packet is a READ one
      #ifdef DEBUG  // Turn it on to print transport level messages
        cout << "Transport READ at 0x" << hex << request.addr << " value ";
        cout << response.data << endl;
      #endif
      response.status = SUCCESS;
      if (request.addr == NUMBER_OF_CORES_REQUEST) {
        response.data = number_of_cores();
      } else { // check if core is on
        response.data = is_core_on((request.addr - ADDRESS_CORE_ZERO_READ) / 4);
      }
      break;
    case WRITE:     // Packet is a WRITE
      #ifdef DEBUG
    cout << "Transport WRITE at 0x" << hex << request.addr << " value ";
    cout << request.data << endl;
      #endif
      response.status = set_core(
        ((request.addr - ADDRESS_CORE_ZERO_WRITE) / 4) % 2,
        (request.addr - ADDRESS_CORE_ZERO_WRITE) / 8
      ) ? SUCCESS : ERROR;
      break;
    default :
      response.status = ERROR;
      break;
    }

    return response;
  }

  /**
   * Default constructor.
   *
   * @param k cores_controller size in kilowords.
   *
   */
  ac_cores_controller( sc_module_name module_name, std::vector<mips*>* cores_ptr );

private:
  std::vector<mips*>* cores;
  std::vector<bool> cores_on_;

};

#endif //_cores_controller_H_
