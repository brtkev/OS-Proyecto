#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lock.h"

void process_runner(int number_vector[BINGO_MAX_NUMBER], struct Player *players[NUM_PLAYERS]);
void bingo_runner();

void show_count(struct Player *players[NUM_PLAYERS], int recaudado)
{
    printf("\nya no quedan suficientes jugadores, Se Acabo el Bingo.\n");
    printf("1. fichas recaudadas: %d\n  monedas reacudadas en total: %d\n", recaudado, recaudado * 10);

    printf("\n2. Número de cartones ganadores por jugador y en total\n");
    int sum = 0;
    int most_winners = 0;
    int less_winners = 10000;

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        printf("    player %d, cartones ganadores: %d\n", players[i]->id, players[i]->cartones_ganadores);
        sum += players[i]->cartones_ganadores;
        if (players[i]->cartones_ganadores > most_winners)
        {
            most_winners = players[i]->cartones_ganadores;
        }
        if (players[i]->cartones_ganadores < less_winners)
        {
            less_winners = players[i]->cartones_ganadores;
        }
    }
    printf("    total: %d\n", sum);

    int id_most_winners[NUM_PLAYERS];
    int id_less_winners[NUM_PLAYERS];
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (players[i]->cartones_ganadores == most_winners)
        {
            id_most_winners[i] = players[i]->id;
        }
        else
        {
            id_most_winners[i] = 0;
        }
        
        if (players[i]->cartones_ganadores == less_winners)
        {
            id_less_winners[i] = players[i]->id;
        }
        else
        {
            id_less_winners[i] = 0;
        }
    }

    printf("\n3. jugador(es) con mas bingos ganados: \n");
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (id_most_winners[i] != 0)
        {
            printf("    player %d con %d bingos\n", id_most_winners[i], most_winners);
        }
    }

    printf("\n4. jugador(es) con menos bingos ganados:\n");
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (id_less_winners[i] != 0)
        {
            printf("    player %d con %d bingos\n", id_less_winners[i], less_winners);
        }
    }
}

void bingo_runner()
{
    int recaudado = 0;
    struct Player *players[NUM_PLAYERS];
    init_players(players);

    int number_vector[BINGO_MAX_NUMBER];

    while (1)
    {
        int round_accumulation = 0;
        reset_random_number_vector(number_vector);
        get_random_number_vector(number_vector);

        // mostrar vector
        printf("number vector created\n");
        // for(int i = 0; i <BINGO_MAX_NUMBER; i++){
        //     printf("%d. %d\n", i, number_vector[i]);
        // }

        prepare_players(players);

        int n_current_players = 0;
        // round_accumulation
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (players[i]->n_cartones > 0)
            {
                n_current_players++;
                int player_deduction = players[i]->n_cartones * 5;
                round_accumulation += player_deduction;
                players[i]->cartera -= player_deduction;
            }
        }

        if (n_current_players < 2)
        {
            show_count(players, recaudado);

            break;
        }

        set_current_players(n_current_players);
        start_locks();
        process_runner(number_vector, players);
        destroy_locks();

        
        int winner = get_winner();
        players[winner - 1]->cartera += round_accumulation * 0.9;
        recaudado += round_accumulation * 0.1;
        printf("ganador de la ultima ronda: player %d\n", winner);
        printf("ganancias del ganador %f\n", round_accumulation * 0.9);
        printf("ganancias del bingo %f\n", round_accumulation * 0.1);

        char c;
        printf("Presione Enter Para la siguiente ronda de Bingo!\n");
        scanf("%c", &c);

    }

    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        free(players[i]);
    }
}

void process_runner(int number_vector[BINGO_MAX_NUMBER], struct Player *players[NUM_PLAYERS])
{
    int thread_count = NUM_PLAYERS + 1;
    HANDLE threads[thread_count];
    DWORD thread_id;

    printf("thread count %d\n", thread_count);

    printf("creating thread %d\n", 0);
    threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)host_function, (PVOID)number_vector, 0, &thread_id);

    if (threads[0] == NULL)
    {

        ExitProcess(1);

        return;
    }
    printf("created thread %d\n", 0);

    for (int i = 1; i < thread_count; i++)
    {
        DWORD thread_id;
        int player_i = i - 1;

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
}

int main()
{

    printf("hello world\n");
    // semilla de numeros random
    time_t t1;
    srand((unsigned)time(&t1));

    bingo_runner();

    return 0;
}