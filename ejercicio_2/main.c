#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lock.h"


void process_runner()
{
    int thread_count = NUM_PLAYERS + 1;
    HANDLE threads[thread_count];
    DWORD thread_id;

    printf("thread count %d\n", thread_count);

    printf("created thread %d\n", 0);
    threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)host_function, (PVOID)0, 0, &thread_id);

    if (threads[0] == NULL)
    {

        ExitProcess(1);

        return;
    }
    
    //arreglo de pointers
    struct Player *players[thread_count];
    
    int vuelta = 0;
    for (int i = 1; i < thread_count; i++)
    {   
        
        DWORD thread_id;
        struct Player player;

        if(vuelta == 0 ){
            //nuevo player
            player.id = i * 2;
            player.score = 0;
            player.n_cartones = get_random_number_from_1_to_10();

            players[i] = &player;
        }

        printf("creating thread %d\n", players[i]->id);
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)player_function, (PVOID)&player, 0, &thread_id);
        printf("created thread %d\n", players[i]->id);
        
        if (threads[i] == NULL)

        {
            printf("Error creando thread %d\n", i);
            ExitProcess(1);

            return;
        }
    }

    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
}

int main()
{

    printf("hello world\n");
    //semilla de numeros random
    time_t t1;
    srand ( (unsigned) time (&t1));
    
    start_locks();

    process_runner();

    destroy_locks();

    return 0;
}