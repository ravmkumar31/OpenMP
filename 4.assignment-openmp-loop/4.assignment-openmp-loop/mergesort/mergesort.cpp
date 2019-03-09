#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
  #endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
  #ifdef __cplusplus
}
#endif


void merge(int * sub_array, int * sub_temp_array, int array_begin, int divid, int array_limit, int n)
{
  if (array_begin == array_limit) return;
  
  int k = array_begin, i = array_begin, j = divid + 1;
  
  while (i <= divid && j <= array_limit)
  {
    if (sub_array[i] < sub_array[j])
      sub_temp_array[k++] = sub_array[i++];
    else
      sub_temp_array[k++] = sub_array[j++];
  }
  
  while (i < n && i <= divid)
    sub_temp_array[k++] = sub_array[i++];
  
  
  #pragma omp parallel for 
  for (int i = array_begin; i <= array_limit; i++)
  { 
    sub_array[i] = sub_temp_array[i];
  } 
    
}


void mergesort(int* local_array, int * local_val, int first_array, int second_array, int n) 
{
  for (int i = 1; i <= second_array - first_array; i = 2*i)
  {
    //#pragma omp parallel for schedule(static)
    #pragma omp parallel for 
    for (int j = first_array; j < second_array; j += 2*i)
    {
      int start = j;
      int mid = j + i - 1;
      int end = std::min(start + 2*i - 1, second_array);

      merge(local_array, local_val, start, mid, end,n );
    }
  }
}

int main (int argc, char* argv[]) 
{
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

  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads);

  int n = atoi(argv[1]);
  
  int * arr = new int [n];
  int * local_array = new int [n];
  
  generateMergeSortData (arr, n);

  for (int i = 0; i < n; i++)
    local_array[i] = arr[i] ;

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  //insert sorting code here.
  
  mergesort(arr, local_array, 0, n - 1,n);
  
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, n);

  delete[] arr;

  return 0;
}