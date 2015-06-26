#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_OF_CORES_REQUEST 5242980
#define ADDRESS_CORE_ZERO_READ 5242981
#define ADDRESS_CORE_ZERO_WRITE 5243080

#define bool char

void set_core(bool on, int core_num) {
  uint32_t* p = (uint32_t*) (ADDRESS_CORE_ZERO_WRITE + 2 * core_num + on);
  *p = on;
}

uint32_t number_of_cores() {
  uint32_t* p = (uint32_t*) NUMBER_OF_CORES_REQUEST;
  return *((uint32_t *) p) >> 24; // número de "zeros" a serem cortados
}

bool is_core_on(int core_num) {
  uint32_t* p = (uint32_t*) (ADDRESS_CORE_ZERO_READ + core_num);
  return (bool)(*((uint32_t *) p) >> 24);
}


int main(int argc, char *argv[]){
  int i;
  // for(i=0;i<10;i++)
  //   printf("Hi from processor MIPS!\n");
  printf("This processor has %" PRIu32 " cores\n", number_of_cores());
  for(i=0;i<8;++i)
    printf("%" PRIu32 " ", is_core_on(i));
  printf("\n");
  set_core(1, 1);
  // set_core(0, 1);
  for(i=0;i<8;++i)
    printf("%" PRIu32 " ", is_core_on(i));
  printf("\n");  

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

