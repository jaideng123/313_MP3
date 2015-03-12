#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)
  /* ========================================================== */
  
    int c;
	int index;
	int b = 8;
	int s = 10000;
	while ((c = getopt (argc, argv, "b:s:")) != -1) {
		switch(c) {
		case 'b':
			b = atoi(optarg);
			break;
		case 's':
			s = atoi(optarg);
			break;
		case '?':
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
			return 1;
		default:
			abort();
		}
	}
	
/* ========================================================== */
  
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
  
  //for getopt failures
  for (index = optind; index < argc; ++index) {
      printf("Non-option argument %s\n", argv[index]);
  }
  return 0;
}
