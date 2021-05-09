#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

struct IncrementArrayParameters
{
    int* arrayElements;
    int start;
    int end;
    int threadNumber;
};

void* IncrementArray(void* params);
void PrintArray(int* arrayName, int* arraySize);

int main()
{
    struct timespec start, end;

    int threads_number;
    printf("Please enter the number of threads you would like to use\n");
    scanf("%d", &threads_number);
    pthread_t* myThreadArray = malloc(sizeof(pthread_t) * threads_number); //creates dynamic array

    int array_size = 1000000; // 1 million

    int number_array[array_size];
    for (int i = 0; i < array_size; i++)
        number_array[i] = i;

    printf("Old Array:\n");
    PrintArray(number_array, &array_size);
    printf("\n\n");

    struct IncrementArrayParameters array_of_structs[threads_number]; //will make 1 struct for each thread
    for (int i = 0; i < threads_number; i++)
    {
        // boundaries are non inclusive on right end
        array_of_structs[i].arrayElements = number_array;
        array_of_structs[i].threadNumber = i;
        array_of_structs[i].start = (i * array_size) / threads_number;
        array_of_structs[i].end = ((i + 1) * array_size) / threads_number;
    }

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

    printf("\n\nNew Array:\n");
    PrintArray(number_array, &array_size);
    printf("\n\n");

    free(myThreadArray);

    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000000ll + end.tv_nsec - start.tv_nsec;
    //microseconds+seconds
    printf("Execution time = %f seconds\nNumber of threads: %d\nNumber of cores: %02d\n", elapsed / 1000000000.0, threads_number, get_nprocs());

    return 0;
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

    printf("Thread %d: worked on [%d] to [%d]\n", a_inf->threadNumber, a_inf->start, a_inf->end);
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