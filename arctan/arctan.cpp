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

/**
 * Computes part of 4 * arctan(1). READ function.
 */
uint32_t ac_arctan::getResult(int core) {
  char f_in[50], f_out[50];
  int threadID, numThreads;
  long double intervals;
  FILE *fp;

  sprintf(f_in, "core%d.out", core);
  if (OFFLOAD_DEBUG) {
    printf("system reading from %s\n", f_in);
  }
  fp = fopen(f_in, "r");
  fscanf(fp, "%d %d %Lf", &threadID, &numThreads, &intervals);
  fclose(fp);
  if (OFFLOAD_DEBUG) {
    printf("%d %d %Lf\n", threadID, numThreads, intervals);
  }

  long double localSum = 0, x, width = 1.0 / intervals;
  int i;
  for(i = threadID; i < intervals; i += numThreads) {
    x = (i + 0.5) * width;
    localSum += 4.0 / (1.0 + x*x);
  }
  if (OFFLOAD_DEBUG) {
    cout << localSum << " " << width << " " << localSum*width << endl;
  }

  sprintf(f_out, "core%d.in", core);
  if (OFFLOAD_DEBUG) {
    printf("system writing to %s\n", f_out);
  }
  fp = fopen(f_out, "w+");
  fprintf(fp, "%32.30Lf\n", localSum * width);
  fclose(fp);
  return 0;
}
