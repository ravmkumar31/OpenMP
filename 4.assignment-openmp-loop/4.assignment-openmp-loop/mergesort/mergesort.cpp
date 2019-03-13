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

void merge(int* arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int* L = new int[n1];
    int* R = new int[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

void mergeSort(int* arr, int l, int r, int numThreads)
{
   //Set the number of threads
   // omp_set_dynamic(0);
    omp_set_num_threads(numThreads);

    int n= r+1;

    for(int level = 1; level < n;level *= 2)
    {
        #pragma omp parallel for schedule(static, 1)
        for(int bindex=0;bindex < n;bindex += (2*level))
        {
            int start = bindex;
            int mid = bindex + (level-1);
            int end = bindex + ((2*level)-1);
            if(mid >= n)
            {
                mid = (bindex+n-1)/2;
                end = n-1;
            }
            else if(end >= n)
            {
                end = n-1;
            }
            merge(arr,start,mid,end);
        }
    }
}

/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}



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

  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int numThreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];

  generateMergeSortData (arr, n);


  //insert sorting code here.
  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  mergeSort(arr, 0, n-1, numThreads);

  //printArray(arr, n);
  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;
  //Print the total execution time (in sec) to the error stream
  cerr<<elapased_seconds.count()<<std::endl;
  checkMergeSortResult (arr, n);

  delete[] arr;

  return 0;
}
