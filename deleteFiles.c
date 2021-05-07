#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

void CreateArray(int threadVersion);
void MainThread();
void OneThread();
void TwoThreads();
void UserDefinedThreads();

char **my_files_to_delete;        /* a 2-D array holding filenames                                                           */
int number_of_files = 200;        /* the number of filenames in the array. Align this number with the files creation script  */
int max_length_of_file_name = 15; /* the maximum length of a filename                                                 */

struct ArrayIndex
{
//    int *arr;
    int start;
    int end;
};

void *DeleteFiles(void *params)
{
    struct ArrayIndex *a_inf = (struct ArrayIndex *)params;
    // int inc_val = 5;
    for (int i = a_inf->start; i < a_inf->end; i++)
    {
        //a_inf->arr[i] = a_inf->arr[i] + inc_val;
        remove(my_files_to_delete[i]);
    }
    //((struct arr_info*)params)->arr = a_inf.arr;
    printf("worked on [%d] to [%d]\n", a_inf->start, a_inf->end);
}

int main()
{
    // 0 -> Main Thread     1 -> One Thread     2 - > Two Threads       3 -> User Defined Threads
    // CreateArray(0);
    // CreateArray(1);
    CreateArray(2);
    // CreateArray(3);
    return 0;
}

void CreateArray(int threadVersion)
{
    /* allocate memory for the rows, the pointers to filenames */
    my_files_to_delete = (char **)malloc(number_of_files * sizeof(char *));

    /* allocate memory for the columns (the actual filenames) */
    for (int i = 0; i < number_of_files; i++)
        my_files_to_delete[i] = (char *)malloc(max_length_of_file_name * sizeof(char));

    /* initialise the array with filenames. Make sure you are using the same format as in the script that creates the files */
    for (int i = 0; i < number_of_files; i++)
        sprintf(my_files_to_delete[i], "%s%d", "file_number_", i); /* this concatenates char* and int and assigns result to my_files_to_delete [i] */

    /* print the filenames to make sure you have the correct names */
    //printf("%s \n", "These are the files that are going to be deleted:");

    // for (int i = 0; i < number_of_files; i++)
    //     printf("\t %s \n", my_files_to_delete[i]);
    
    switch (threadVersion)
    {
    case 0: MainThread(); break;
    case 1: OneThread(); break;
    case 2: TwoThreads(); break;
    case 3: UserDefinedThreads(); break;
    }

    //free memory
    for (int i = 0; i < number_of_files; i++)
        free(my_files_to_delete[i]);

    free(my_files_to_delete);
}

void MainThread()
{
    /* remove the files. This is the part you need to make parallel */
    for (int i = 0; i < number_of_files; i++)
        remove(my_files_to_delete[i]); /* you can test the return of remove if you want to check if remove failed/succeeded */
}

void OneThread()
{
    struct timespec start, end;
    pthread_t deleteFilesThread;

    struct ArrayIndex threadStruct;
    threadStruct.start = 0;
    threadStruct.end = number_of_files;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_create(&deleteFilesThread, NULL, &DeleteFiles, (void *)&threadStruct);
    pthread_join(deleteFilesThread, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000000ll + end.tv_nsec - start.tv_nsec;

    //microseconds+seconds
    printf("Execution time = %f seconds\n", elapsed / 1000000000.0);
}

void TwoThreads()
{
    int numberOfThreads = 2;
    struct timespec start, end;
    pthread_t deleteFilesThread[numberOfThreads];

    struct ArrayIndex threadStructs[2];
    for (int i = 0; i < numberOfThreads; i++)
    {
        // boundaries are non inclusive on right end
        threadStructs[i].start = (i * number_of_files) / numberOfThreads;
        threadStructs[i].end = ((i + 1) * number_of_files) / numberOfThreads;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < numberOfThreads; i++)
    {
        pthread_create(&deleteFilesThread[i], NULL, &DeleteFiles, (void *)&threadStructs[i]);
    }
    for (int i = 0; i < numberOfThreads; i++)
    {
        pthread_join(deleteFilesThread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000000ll + end.tv_nsec - start.tv_nsec;

    //microseconds+seconds
    printf("Execution time = %f seconds\n", elapsed / 1000000000.0);
}

void UserDefinedThreads()
{
    struct timespec start, end;

    int threads_number;
    printf("Please enter the number of threads you would like to use\n");
    scanf("%d", &threads_number);
    pthread_t *myThreadArray = malloc(sizeof(pthread_t) * threads_number); //creates dynamic array

    struct ArrayIndex array_of_structs[threads_number]; //will make 1 struct for each thread
    for (int i = 0; i < threads_number; i++)
    {
        // boundaries are non inclusive on right end
        array_of_structs[i].start = (i * number_of_files) / threads_number;
        array_of_structs[i].end = ((i + 1) * number_of_files) / threads_number;
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < threads_number; i++)
    {
        //initializes threads
        pthread_create(&myThreadArray[i], NULL, DeleteFiles, (void *)&array_of_structs[i]);
    }

    for (int i = 0; i < threads_number; i++)
    {
        //waits for threads
        pthread_join(myThreadArray[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // for (int i = 0; i < number_of_files; i++)
    // {
    //     printf("array at %d is %d\n", i, number_array[i]);
    // }

    free(myThreadArray);

    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000000ll + end.tv_nsec - start.tv_nsec;

    //microseconds+seconds
    printf("Execution time = %f seconds\n", elapsed / 1000000000.0);
}