#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242981
#define ADDRESS_CORE_ZERO_WRITE 115243080

#define bool char

void set_core(bool on, int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_WRITE + 2 * core_num + on);
  *p = on;
}

uint32_t number_of_cores() {
  uint32_t volatile* p = (uint32_t volatile*) NUMBER_OF_CORES_REQUEST;
  return *p >> 24; // número de "zeros" a serem cortados
}

bool is_core_on(int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_READ + core_num);
  return (bool)(*p >> 24);
}


int main(int argc, char *argv[]){
  int i;
  // for(i=0;i<10;i++)
  //   printf("Hi from processor MIPS!\n");
  printf("This processor has %" PRIu32 " cores\n", number_of_cores());
  for(i=0;i<8;++i)
    printf("%" PRIu32 " ", is_core_on(i));
  printf("\n");
  for(i=1;i<8;++i)
    set_core(1,i);
  for(i=0;i<8;++i)
    printf("%" PRIu32 " ", is_core_on(i));
  printf("\n");  

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

