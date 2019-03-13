#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

using namespace std;

void merge(int* arr, int begin, int center, int last)
{
    int  i,j,k,midterm = center - begin + 1,endterm =  last - center;
    //ilising two arrays by diving almost into two halves
    int* first_half = new int[midterm];
    int* second_half = new int[endterm];

    //get data into this arrays for computation
    for (int i = 0; i < midterm; i++)
        first_half[i] = arr[begin + i];
    for (int j = 0; j < endterm; j++)
        second_half[j] = arr[center + 1+ j];

    //Merge the two arrays and get back to main array
    i = 0;
    j = 0;
    k = begin;
    while (i < midterm && j < endterm)
    {
        if (first_half[i] <= second_half[j])
        {
            arr[k] = first_half[i];
            i++;
        }
        else
        {
            arr[k] = second_half[j];
            j++;
        }
        k++;
    }
    //get the left out elements from first half array
    while (i < midterm)
    {
        arr[k] = first_half[i];
        i++;
        k++;
    }

    //get the left out elements from second half array
    while (j < endterm)
    {
        arr[k] = second_half[j];
        j++;
        k++;
    }
    delete[] first_half;
    delete[] second_half;
}

void mergeSort(int* arr, int begin, int last, int nbthreads)
{
    //creating number of threads
    omp_set_num_threads(nbthreads);
    int n= last+1;
    for(int level = 1; level < n;level *= 2)
    {
        #pragma omp parallel for schedule(static)
        for(int k=0;k < n;k += (2*level))
        {
            int initial = k;
            int center = k + (level-1);
            int rear = k + ((2*level)-1);
            if(center >= n)
            {
                center = (k+n-1)/2;
                rear = n-1;
            }
            else if(rear >= n)
            {
                rear = n-1;
            }
            merge(arr,initial,center,rear);
        }
    }
}




int main (int argc, char* argv[]) {

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

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];

  generateMergeSortData (arr, n);


  std::chrono::time_point<std::chrono::system_clock> clock_start = std::chrono::system_clock::now();//time begins
  mergeSort(arr, 0, n-1, nbthreads);//Runs merge sort 
  std::chrono::time_point<std::chrono::system_clock> clock_end = std::chrono::system_clock::now();//time ends when computation is done
  std::chrono::duration<double> total_time = clock_end-clock_start;
  cerr<<total_time.count()<<std::endl;
  checkMergeSortResult (arr, n);

  delete[] arr;

  return 0;
}
