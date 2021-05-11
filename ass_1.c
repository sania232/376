#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

string FILENAME "ThreadExecutionReport.csv"

long long AVG_TOTAL = 0;
int EXECUTION_COUNT = 6; // to get the average execution time for each number of thread
FILE* filePointer;

struct IncrementArrayParameters
{
    int* arrayElements;
    int start;
    int end;
    int threadNumber;
};

void ExecuteThreads(int threads_number, int execution_number);
void* IncrementArray(void* params);
void PrintArray(int* arrayName, int* arraySize);
void RecordOutputToFile(int threadNumber, int executionNumber, long long elapsed);

int main()
{	
	
    filePointer = fopen(FILENAME, "w+");
    int threads_number, start_thread_number = 1, end_thread_number = 6;
    printf("Please enter the number of threads you would like to start from\n");
    scanf("%d", &start_thread_number);
    printf("Please enter the number of threads you would like to end at\n");
    scanf("%d", &end_thread_number);
    threads_number = start_thread_number;



    system("clear");

    for (int i = start_thread_number; i < (end_thread_number + 1); i++)
    {
        for (int j = 0; j < EXECUTION_COUNT; j++)
        {
            ExecuteThreads(threads_number, (j + 1));
        }
        printf("--------------------------------------------\n");
        threads_number++;
    }

	 fclose(filePointer);

    return 0;
}

/* Function: Execute Threads
*  Usage: ExecuteThreads(threads_number);
*  --------------------------
*  Function to create and execute threads based on the parameter passed.
*/
void ExecuteThreads(int threads_number, int execution_number)
{
    pthread_t* myThreadArray = malloc(sizeof(pthread_t) * threads_number); //creates dynamic array

    int array_size = 1000000; // Tested on 100, 1000, 10,000, 100,000 and 1 million

    int number_array[array_size];
    for (int i = 0; i < array_size; i++)
        number_array[i] = i;

    /* TEST: To check whether array is filled with elements*/
    // printf("Old Array:\n");
    // (number_array, &array_size);
    // printf("\n\n");

    struct IncrementArrayParameters array_of_structs[threads_number]; //will make 1 struct for each thread
    for (int i = 0; i < threads_number; i++)
    {
        // boundaries are non inclusive on right end
        array_of_structs[i].arrayElements = number_array;
        array_of_structs[i].threadNumber = i;
        array_of_structs[i].start = (i * array_size) / threads_number;
        array_of_structs[i].end = ((i + 1) * array_size) / threads_number;
    }

    printf("Execution Number: %d\nNumber of Threads: %d\n", execution_number, threads_number);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < threads_number; i++)
    {
        //initializes threads
        pthread_create(&myThreadArray[i], NULL, IncrementArray, (void*)&array_of_structs[i]);
    }
    for (int i = 0; i < threads_number; i++)
    {
        //waits for threads
        pthread_join(myThreadArray[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    /* TEST: To check whether all array elements are incremented by 5*/
    // printf("\n\nNew Array:\n");
    // PrintArray(number_array, &array_size);
    // printf("\n\n");

    free(myThreadArray);

    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000000ll + end.tv_nsec - start.tv_nsec;
    //microseconds+seconds
    printf("Execution time = %f seconds\nNumber of cores: %02d\n\n\n", (elapsed / 1000000000.0), get_nprocs());
    RecordOutputToFile(threads_number, execution_number, elapsed);
}

/* Function: Increment Array
*  Usage: IncrementArray((void*)&parameter);
*  -------------------------------------------------
*  Function to increment array
*/
void* IncrementArray(void* params)
{
    struct IncrementArrayParameters* a_inf = (struct IncrementArrayParameters*)params;
    int inc_val = 5;

    for (int i = a_inf->start; i < a_inf->end; i++)
        a_inf->arrayElements[i] += inc_val;

    printf("Thread %d: worked on [%d to %d)\n", a_inf->threadNumber, a_inf->start, a_inf->end);
}

/* Function: Print Array
*  Usage: PrintArray(integerArrayName, &arraySize);
*  -------------------------------------------------
*  Function to print integer array
*/
void PrintArray(int* arrayName, int* arraySize)
{
    int totalNumsInLine = 1;
    printf("{\n");
    for (int i = 0; i < *arraySize; i++)
    {
        if (i == (*arraySize - 1))
        {
            printf("%04d\n}\n", *arrayName + i);
            break;
        }
        if (totalNumsInLine == 25)
        {
            printf("%04d,\n", *arrayName + i);
            totalNumsInLine = 0;
        }
        else
        {
            printf("%04d, ", *arrayName + i);
        }
        totalNumsInLine++;
    }
}

/* Function: Record Output To File
*  Usage: RecordOutputToFile(numberOfThreadas, executionNumber, timeElapsed)
*  -------------------------------------------------
*  Function to write the execution time and other details into a csv file properly formatted
*/
void RecordOutputToFile(int number_of_threads, int executionNumber, long long elapsed)
{
   
    if (filePointer == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fseek(filePointer, 0, SEEK_END); // place the pointer at the start and iterate till it reaches the null character in the file
    int size = ftell(filePointer); // get number of char in file
    if (size == 0) // file is empty so add the column header names
    {
        fprintf(filePointer, "Core(s),# of Threads,Execution Number,μs,Seconds,Average μs\n");
    }

    fprintf(filePointer, "%d,%d,%d,%04lld,%f", get_nprocs(), number_of_threads, executionNumber, (elapsed / 1000), ((double)elapsed / 1000000000.0));

    if (executionNumber == EXECUTION_COUNT)
    {	
    	 AVG_TOTAL += (elapsed / 1000);
        fprintf(filePointer, ",%04lld\n", (AVG_TOTAL / EXECUTION_COUNT));
        //fclose(filePointer);
        AVG_TOTAL = 0;
    }
    else
    {
        fprintf(filePointer, ",\n");
        AVG_TOTAL += (elapsed / 1000);
       
    }
}
