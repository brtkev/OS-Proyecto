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
    struct Player *players[NUM_PLAYERS];
    
    int vuelta = 0;
    for (int i = 1; i < thread_count; i++)
    {   
        
        DWORD thread_id;
        int player_i = i - 1;

        if(vuelta == 0 ){
            //nuevo player
            // struct Player new_player;
            players[player_i] = (struct Player*)malloc(sizeof(struct Player));
            players[player_i]->id = i;
            players[player_i]->score = 0;
            players[player_i]->current_number_count = 0;
            players[player_i]->n_cartones = get_random_number_from_1_to_10();

        }

        printf("creating thread %d\n", players[player_i]->id);
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)player_function, (PVOID)players[player_i], 0, &thread_id);
        printf("created thread %d\n", players[player_i]->id);

        if (threads[i] == NULL)

        {
            printf("Error creando thread %d\n", i);
            ExitProcess(1);

            return;
        }
    }

    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
    for( int i = 0 ; i < NUM_PLAYERS; ++i){
        free(players[i]);
    }
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