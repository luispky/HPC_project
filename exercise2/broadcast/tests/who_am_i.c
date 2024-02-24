#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define DEFAULT_NUM_THREADS 4

void who_am_i(int* thread_num){
  *thread_num = omp_get_thread_num();
}

int main(int argc, char* argv[]){
  int num_threads = (argc>1) ? atoi(*(argv+1)) : DEFAULT_NUM_THREADS;
  omp_set_num_threads(num_threads); 
  
  int thread_num;
  #pragma omp parallel
  {
    who_am_i(&thread_num);
    printf("Hello from thread %d\n", thread_num);
  }
  return 0;
}


