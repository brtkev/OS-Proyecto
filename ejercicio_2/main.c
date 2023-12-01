#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lock.h"


void process_runner()
{
    // https://codeforwin.org/c-programming/how-to-find-first-and-last-digit-of-any-number
    // int thread_count = rand() % 10;
    int thread_count = NUM_PLAYERS;
    HANDLE threads[thread_count + 1];
    DWORD i;

    for (i = 1; i < thread_count; i++)
    {
        DWORD thread_id;
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)player_function, (PVOID)i, 0, &thread_id);

        if (threads[i] == NULL)

        {

            ExitProcess(1);

            return;
        }
    }

    DWORD thread_id;
    threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)host_function, (PVOID)i, 0, &thread_id);

    if (threads[0] == NULL)
    {

        ExitProcess(1);

        return;
    }

    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
}

int main()
{
    //semilla de numeros random
    time_t t1;
    srand ( (unsigned) time (&t1));

    printf("hola mundo\n");
    process_runner();

    return 0;
}