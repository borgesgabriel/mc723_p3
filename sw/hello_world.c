#include <stdio.h>
#include <stdlib.h>

bool set_core(bool on, int core_num) {
  
}

int number_of_cores() {
  return 
}

bool is_core_on(int core_num) {
  return cores_on_.at(core_num);
}


int main(int argc, char *argv[]){
  int i;
  for(i=0;i<10;i++)
    printf("Hi from processor MIPS!\n");

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

