#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <chrono>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int *thread_local_sum;
  int n = atoi(argv[1]);

  int * arr = new int [n];
  generatePrefixSumData (arr, n);

  int * pr = new int [n+1];

  //insert prefix sum code here

  int nbthreads = atoi(argv[2]);

  auto clock_start = std::chrono::system_clock::now();
  pr[0] = arr[0];
  #pragma omp parallel
  {
    int thread_num = omp_get_thread_num();

    #pragma omp single
    {
      thread_local_sum = new int[nbthreads+1];
      pr[0] = 0;
    }

    int thread_sum = 0;
    #pragma omp for schedule(auto) nowait 
    for (int j = 0; j < n; ++j)
    {
      thread_sum += arr[j];
      pr[j+1] = thread_sum;
    }
    thread_local_sum[thread_num+1];

    #pragma omp barrier
    int x = 0;
    for(int i=0; i<(thread_num+1); i++) {
      x += thread_local_sum[i];
    }

    #pragma omp for schedule(auto) 
    for(int i=0; i<n; i++) {
      pr[i+1] += x;
    }

  }
  auto clock_end = std::chrono::system_clock::now();
  std::chrono::duration<double>diff = clock_end - clock_start;
  std::cerr<<diff.count();

  checkPrefixSumResult(pr, n);

  delete[] arr;

  return 0;
}
