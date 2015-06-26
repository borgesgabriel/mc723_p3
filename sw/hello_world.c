#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_OF_CORES_REQUEST 5242980
#define ADDRESS_CORE_ZERO_READ 0x1
#define ADDRESS_CORE_ZERO_WRITE 0x2

// bool set_core(bool on, int core_num) {
  
// }

uint32_t number_of_cores() {
  uint32_t* p = (uint32_t*) NUMBER_OF_CORES_REQUEST;
  return *((uint32_t *) p) >> 24; // número de "zeros" a serem cortados
}

// bool is_core_on(int core_num) {
//   return cores_on_.at(core_num);
// }


int main(int argc, char *argv[]){
  int i;
  for(i=0;i<10;i++)
    printf("Hi from processor MIPS!\n");
  printf("%" PRIu32 "\n", number_of_cores());

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

