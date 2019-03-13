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

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  //insert code here
  //Declare the input variables to compute the numrical integration
  int functionid = atoi(argv[1]);
  int a = atoi(argv[2]);
  int b = atoi(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]);
  char *schedule_type = argv[7];
  int granularity = atoi(argv[8]);
  float local_xval,numintegration_sum = 0, sub_sum;

  //calculate the (b-a)/n and save it to subsome
  sub_sum = (((float)b - (float)a)/(float)n);
  //create threads to compute parallel computing
  omp_set_num_threads(nbthreads); 

  auto clock_start = std::chrono::system_clock::now();//begin the clock
   
  if(strcmp(schedule_type , "static") == 0)//check which scheduling is selected by user
  {
    if (granularity < 0)//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(static) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
    else//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(static,granularity) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
  }
  else if(strcmp(schedule_type , "dynamic") == 0) //check which scheduling is selected by user
  {
    if (granularity < 0)//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(dynamic) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
    else//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(dynamic,granularity) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
  } 
  else if(strcmp(schedule_type , "guided") == 0)//check which scheduling is selected by user 
  {
    if (granularity < 0)//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(guided) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
    else//granularity heck if less than 0 it than change logic for chunksize.If its negative just implement without chunksize
    {
      #pragma omp parallel for schedule(guided,granularity) reduction(+:numintegration_sum)//initiate paralleism with reduction on integrationsum
      //below code is sakme code we implement in sequential code
      for (int i = 0; i < n; ++i)
      {
        local_xval = (float)a + (((float)i + 0.5)*sub_sum);
        switch(functionid)
        {
          case 1:
            numintegration_sum += f1(local_xval,intensity);
          break;
          case 2:
            numintegration_sum += f2(local_xval,intensity);
          break;
          case 3:
            numintegration_sum += f3(local_xval,intensity);
          break;
          case 4:
            numintegration_sum += f4(local_xval,intensity);
          break;
          default: std::cout<<"Please Enter correct Function ID"<<std::endl;
        }
      }
    }
  }  
  else
  {
    std::cout<<"Please Enter correct Sceduling Type"<<std::endl;
  }

  auto clock_end = std::chrono::system_clock::now();
  std::chrono::duration<double>diff = clock_end - clock_start;
  std::cout<<numintegration_sum*sub_sum<<std::endl;
  std::cerr<<diff.count();

  return 0;
}
