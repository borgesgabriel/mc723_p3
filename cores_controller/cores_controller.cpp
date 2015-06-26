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
ac_cores_controller::ac_cores_controller( sc_module_name module_name, std::vector<mips*>* cores_ptr ) :
  sc_module( module_name ),
  target_export("iport")
{
    cores = cores_ptr;
    cores_on_.resize(number_of_cores());
    cores_on_[0] = true;
    /// Binds target_export to the cores_controller
    target_export( *this );
}

bool ac_cores_controller::set_core(bool on, int core_num) {
  if (is_core_on(core_num) == on || core_num < 0 || core_num >= number_of_cores()) {
    return false;
  }
  if (on) {
    cores_on_.at(core_num) = true;
    cores->at(core_num)->ISA.ResumeProcessor();
  } else {
    cores_on_.at(core_num) = false;
    cores->at(core_num)->ISA.PauseProcessor();
  }
  return true;
}

int ac_cores_controller::number_of_cores() {
  return cores->size();
}

bool ac_cores_controller::is_core_on(int core_num) {
  if (core_num < 0 || core_num >= number_of_cores()) {
    return 0;
  }
  return cores_on_.at(core_num);
}


