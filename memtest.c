#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)

  init_allocator(8, 65);
  print_free_lists();
  split(0);
  print_free_lists();
  split(1);
  print_free_lists();
  split(1);
  print_free_lists();
  split(2);
  print_free_lists();
  split(2);
  print_free_lists();
  consolidate();
  print_free_lists();
  //ackerman_main();

  release_allocator();
}
