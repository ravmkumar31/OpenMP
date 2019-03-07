#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif
  void generateReduceData (int* arr, size_t n);
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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int nbthreads = atoi(argv[2]);
  char *schedule_type = argv[3];
  int granularity = atoi(argv[4]);
  int reduced_sum = 0;
  generateReduceData (arr, atoi(argv[1]));

  
  omp_set_num_threads(nbthreads); 

  //insert reduction code here
  auto clock_start = std::chrono::system_clock::now();
  if(strcmp(schedule_type , "static") == 0)
  {
    if (granularity<0)
    {
       #pragma omp parallel for schedule(static) reduction(+:reduced_sum)
        for (int i = 0; i < n; ++i)
        {
          reduced_sum+=arr[i];
        }  
    }
    else
    {
      #pragma omp parallel for schedule(static,granularity) reduction(+:reduced_sum)
      for (int i = 0; i < n; ++i)
      {
        reduced_sum+=arr[i];
      }
    }  
  }
  else if(strcmp(schedule_type, "dynamic") == 0)
  {
    if (granularity<0)
    {  
     #pragma omp parallel for schedule(dynamic) reduction(+:reduced_sum)
     for (int i = 0; i < n; ++i)
     {
       reduced_sum+=arr[i];
     }
    }
    else
    {
      #pragma omp parallel for schedule(dynamic,granularity) reduction(+:reduced_sum)
      for (int i = 0; i < n; ++i)
      {
       reduced_sum+=arr[i];
      }
    } 
  }
  // else if (strcmp(schedule_type,"guided") == 0)
  // {
  //   if (granularity<0)
  //   {
  //     #pragma omp parallel for schedule(guided) reduction(+:reduced_sum)
  //     for (int i = 0; i < n; ++i)
  //     {
  //       reduced_sum+=arr[i];
  //     } 
  //   }
  //   else
  //   {
  //     #pragma omp parallel for schedule(guided,granularity) reduction(+:reduced_sum)
  //     for (int i = 0; i < n; ++i)
  //     {
  //       reduced_sum+=arr[i];
  //     } 
  //   }
  // }
  
  
  auto clock_end = std::chrono::system_clock::now();
  std::chrono::duration<double>diff = clock_end - clock_start;
  std::cout<<reduced_sum<<std::endl;
  std::cerr<<diff.count();
  delete[] arr;

  return 0;
}
