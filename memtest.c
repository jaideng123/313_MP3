#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)

  init_allocator(4, 8000);
  //consolidate();
  //print_free_lists();
  ackerman_main();

  release_allocator();
}
