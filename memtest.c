#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)

  init_allocator(8, 1000000);
  print_free_lists();
  //consolidate();
  Addr a[7];
  a[0] = my_malloc(4);
  a[1] = my_malloc(88);
  a[2] = my_malloc(18350);
  a[3] = my_malloc(38010);
  a[4] = my_malloc(4);
  a[5] = my_malloc(7);
  a[6] = my_malloc(91750);
  int i;
  for(i = 0; i<7; ++i){
    my_free(a[i]);
  }
  consolidate();
  print_free_lists();
  a[0] = my_malloc(4);
  a[1] = my_malloc(88);
  a[2] = my_malloc(18350);
  a[3] = my_malloc(38010);
  a[4] = my_malloc(4);
  a[5] = my_malloc(7);
  a[6] = my_malloc(91750);
  for(i = 0; i<7; ++i){
    my_free(a[i]);
  }

  //ackerman_main();

  release_allocator();
}
