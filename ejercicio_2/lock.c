
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lock.h"

// Create two semaphores: one for admins and one for writers.
HANDLE host_semaphore;
HANDLE player_semaphore;
HANDLE bingo_semaphore;

int num_players_not_ready;
int winner = 0;
int current_number;

void start_locks()
{
    // Initialize the semaphores.
    printf("SET - Initialize the semaphores\n");
    host_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
    player_semaphore = CreateSemaphore(NULL, 0, NUM_PLAYERS, NULL);
    bingo_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    num_players_not_ready = NUM_PLAYERS;

}

void destroy_locks()
{
    // Destroy the semaphores
    printf("SET - destroy the semaphores\n");
    CloseHandle(host_semaphore);
    CloseHandle(player_semaphore);
    CloseHandle(bingo_semaphore);
}


void player_function(struct Player * player_ptr)
{
    time_t t = player_ptr->id;
    srand ( (unsigned) time (&t));
    struct Player player = *player_ptr;

    printf("player id %d\n", player.id);

    printf("player %d is getting %d cartons\n", player.id, player.n_cartones);

    fill_player_cartones(player_ptr);
    if(player.id == 2){
        checkout_player_bingo(player_ptr);
    }

    return;

    printf("player %d got his %d cartons filled\n", player.id, player.n_cartones);

    for (winner; winner == 0; winner)
    {
        // signal( al gordo )
        --num_players_not_ready;
        printf("player %d esta listo, el numero de jugadores que no estan listos es %d\n", player.id, num_players_not_ready);
        if (num_players_not_ready == 0)
        {
            printf("player %d avisa al host que todos los jugadores estan listos\n", player.id);
            ReleaseSemaphore(host_semaphore, 1, NULL);
        }

        // wait ( por el gordo que diga numero)
        WaitForSingleObject(player_semaphore, INFINITE);

        ++num_players_not_ready;
        printf("player %d esta leyendo sus cartones buscando el numero %d\n", player.id, current_number);

        // BINGO
        if (is_bingo_in_player(player) == 1)
        { 
            printf("player %d tiene bingo\n", player.id);
            // wait (esperan otros jugadores)
            WaitForSingleObject(bingo_semaphore, INFINITE);
            //  si nadie ha avisado
            if (winner == 0)
            {
                printf("player %d avisa que tiene bingo\n", player.id);
                // avisa al gordo que gana
                winner = player.id;
            }
            // signal( libera otros jugadores)
            ReleaseSemaphore(bingo_semaphore, 1, NULL);
        }
    }

    printf("player %d termino de jugar\n", player.id);
}

void fill_player_cartones(struct Player * player)
{
    // llenar cartones
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {

            player->cartones[carton][space][NUMBERS_CARTON] = get_random_number(1, BINGO_MAX_NUMBER);
            // printf("random number: %d\n", player.cartones[carton][space][NUMBERS_CARTON]);
            player->cartones[carton][space][BINGO_CARTON] = 0;
            // printf("bingo carton: %d\n", player.cartones[carton][space][BINGO_CARTON]);
        }
        player->cartones_winner_count[carton] = 0;
    }
    
}

int is_bingo_in_player(struct Player player)
{
    for (int carton = 0; carton < player.n_cartones; ++carton)
    {
        if (is_bingo_in_carton(player, carton))
            return 1;
    }
}

int is_bingo_in_carton(struct Player player, int carton)
{
    for (int number_position = 0; number_position < BINGO; ++number_position)
    {

        if (is_bingo(player, carton, number_position))
            return 1;
    }
}

int is_bingo(struct Player player, int carton, int number_position)
{
    if (current_number == player.cartones[carton][number_position][NUMBERS_CARTON])
    {
        if (player.cartones[carton][number_position][BINGO_CARTON] == 1)
            return 0;

        ++player.cartones_winner_count[carton];
        if (player.cartones_winner_count[carton] == BINGO){
            printf("player %d, carton %d, cantidad de winners %d", player.id, carton, player.cartones_winner_count);
            return 1;
        }

        player.cartones[carton][number_position][BINGO_CARTON] = 1;

        return 0;
    }
}

void host_function(DWORD i)
{
    time_t t = i;
    srand ( (unsigned) time (&t));

    // hasta que alguien gane
    for (winner; winner == 0; winner)
    {

        int new_number = get_random_number(1, BINGO_MAX_NUMBER);

        printf("el host tiene un numero nuevo, esperando a que todos los jugadores esten listos\n");
        // wait ( que todos los players tengan cartones llenos)
        // wait( que todos hayan buscado el numero en sus cartones)
        WaitForSingleObject(host_semaphore, INFINITE);

        if (winner != 0)
        {
            printf("BINGO BY PLAYER %d\n", winner);
            // envia plata a la cartera del jugador ganador
        }
        else
        {
            // asignar nuevo numero
            printf("el nuevo numero es %d\n", new_number);
            current_number = new_number;

            // signal( nuevo numero)
            // solo hace signal si tiene minimo NUM_PLAYERS espacios
            for (; !ReleaseSemaphore(player_semaphore, NUM_PLAYERS, NULL);)
                ;
        }
    }

    // reinicia juego

    printf("host cierra el bingo %d\n", i);
}

int get_random_number(int from, int to)
{
    int delta = 100;
    if( from > to ) return -1;

    int new_number = rand() % delta;
    // validacion 
    // no mayor que to 
    // y
    // no menor que from
    for (new_number; new_number > to || new_number < from; new_number = rand() % delta)
        ;


    return new_number;
}

int get_random_number_from_1_to_10(){
    int new_number = rand() % 10;
    
    if( new_number == 0) {
        new_number = 10;
    }

    return new_number;
}


void checkout_player_bingo(struct Player * p){
    printf("player %d con %d cartones\n\n", p->id, p->n_cartones);
    for(int i = 0; i < p->n_cartones; ++i){
        for( int j = 0; j < BINGO; ++j){
            printf("carton %d, espacio %d, valor %d \n", i, j, p->cartones[i][j][NUMBERS_CARTON]);
            printf("carton %d, espacio %d, status %d \n", i, j, p->cartones[i][j][BINGO_CARTON]);
        }
        printf("carton %d, winners %d\n\n", i, p->cartones_winner_count[i]);  
    }
}