#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>



struct arr_info{
 int* arr; int start;int end;
 };

void* incrementArray(void* params){
	struct arr_info* a_inf=(struct arr_info*)params;
	int inc_val=5;
	for(int i=a_inf->start;i<a_inf->end;i++){
		a_inf->arr[i]=a_inf->arr[i]+inc_val;
	}
	//((struct arr_info*)params)->arr = a_inf.arr;
	printf("worked on [%d] to [%d]\n",a_inf->start,a_inf->end);
}

int main()
{	
	struct timespec  start, end;
	
	int threads_number;
   	printf("Please enter the number of threads you would like to use\n");
   	scanf("%d",&threads_number);
  	pthread_t* myThreadArray=malloc(sizeof(pthread_t)*threads_number);//creates dynamic array
  	
  	int array_size=1000;
  	
	int number_array[array_size];
	for(int i=0;i<array_size;i++){
		number_array[i]=i;
	}
	
	
	
	struct arr_info array_of_structs[threads_number];//will make 1 struct for each thread
        for(int i=0;i<threads_number;i++){
        	// boundaries are non inclusive on right end
    	 	array_of_structs[i].arr=number_array;
    	 	array_of_structs[i].start=(i*array_size)/threads_number;
    	 	array_of_structs[i].end=((i+1)*array_size)/threads_number;
    	 
       }
       
       clock_gettime(CLOCK_MONOTONIC,&start);
    	for(int i=0;i<threads_number;i++){
    		//initializes threads
    		pthread_create(&myThreadArray[i],NULL,incrementArray,(void*)&array_of_structs[i]);
    	}
    	
	
	for(int i=0;i<threads_number;i++){
		//waits for threads
	    	pthread_join(myThreadArray[i],NULL);
	    	
	 }
	 
	  clock_gettime(CLOCK_MONOTONIC,&end);
	 
	 for(int i=0;i<array_size;i++){
		printf("array at %d is %d\n",i,number_array[i]);
	 
	}
	 
	 free(myThreadArray);
	 
	 long long elapsed=(end.tv_sec-start.tv_sec)*1000000000ll+end.tv_nsec-start.tv_nsec;
         
         //microseconds+seconds
	printf ("Execution time = %f seconds\n",elapsed/1000000000.0);
         
	
	
	 return 0;
}