
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lock.h"

// Create two semaphores: one for admins and one for writers.
HANDLE host_semaphore;
HANDLE player_semaphore;
HANDLE bingo_semaphore;

int num_players_ready;
int winner;
int current_number;
int current_number_count;
int current_players;

void set_current_players(int n){
    current_players = n;
}

void start_locks()
{
    // Initialize the semaphores.
    printf("SET - Initialize the semaphores\n");
    host_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
    player_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    bingo_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    num_players_ready = 0;
    current_number_count = 0;
    winner = 0;
}

void destroy_locks()
{
    // Destroy the semaphores
    printf("SET - destroy the semaphores\n");
    CloseHandle(host_semaphore);
    CloseHandle(player_semaphore);
    CloseHandle(bingo_semaphore);
}

void player_function(struct Player *player_ptr)
{
    if(player_ptr->n_cartones == 0){
        return;
    }
    time_t t = player_ptr->id * 3.1415;
    srand((unsigned)time(&t));

    printf("player id %d\n", player_ptr->id);

    printf("player %d is getting %d cartons\n", player_ptr->id, player_ptr->n_cartones);

    // llena el jugador
    //  fill_player_cartones(player_ptr);

    // muestra todas las tarjetas del jugador
    //  checkout_player_bingo(player_ptr);

    printf("player %d got his %d cartons filled\n", player_ptr->id, player_ptr->n_cartones);
    ++num_players_ready;

    for (winner; winner == 0; winner)
    {
        // wait ( por el gordo que diga numero)
        WaitForSingleObject(player_semaphore, INFINITE);

        if (player_ptr->current_number_count < current_number_count)
        {

            ++player_ptr->current_number_count;

            printf("player %d esta leyendo sus cartones buscando el numero %d\n", player_ptr->id, current_number);

            // BINGO
            if (is_bingo_in_player(player_ptr) == 1)
            {
                printf("player %d tiene bingo\n", player_ptr->id);
                // wait (esperan otros jugadores)
                WaitForSingleObject(bingo_semaphore, INFINITE);
                //  si nadie ha avisado
                if (winner == 0)
                {
                    printf("\n--- player %d avisa que tiene bingo ---\n", player_ptr->id);
                    checkout_player_bingo(player_ptr);
                    // avisa al gordo que gana
                    player_ptr->cartones_ganadores++;
                    winner = player_ptr->id;
                }
                // signal( libera otros jugadores)
                ReleaseSemaphore(bingo_semaphore, 1, NULL);
            }

            // signal( al gordo )
            ++num_players_ready;
            printf("player %d esta listo, el numero de jugadores que estan listos es %d\n", player_ptr->id, num_players_ready);
        }

        if (num_players_ready == current_players)
        {
            printf("player %d avisa al host que todos los jugadores estan listos\n", player_ptr->id);
            ReleaseSemaphore(host_semaphore, 1, NULL);
        }
        ReleaseSemaphore(player_semaphore, 1, NULL);
    }

    // FIN
    ReleaseSemaphore(host_semaphore, 1, NULL);
    printf("player %d termino de jugar\n", player_ptr->id);
}

void fill_player_cartones(struct Player *player)
{
    // llenar cartones
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {
            int new_number = get_random_number(1, BINGO_MAX_NUMBER);

            // si el numero esta duplicado, reinicia el loop
            if (number_not_in_carton(player, carton, new_number) != 1)
            {
                --space;
                continue;
            }

            player->cartones[carton][space][NUMBERS_CARTON] = new_number;
            // printf("random number: %d\n", player.cartones[carton][space][NUMBERS_CARTON]);
            player->cartones[carton][space][BINGO_CARTON] = 0;
            // printf("bingo carton: %d\n", player.cartones[carton][space][BINGO_CARTON]);
        }
        player->cartones_winner_count[carton] = 0;
    }
}

int number_not_in_carton(struct Player *p, int carton, int number)
{
    for (int i = 0; i < BINGO; ++i)
    {
        if (p->cartones[carton][i][NUMBERS_CARTON] == number)
        {
            return 0;
        }
    }
    return 1;
}

int is_bingo_in_player(struct Player *player)
{
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        if (is_bingo_in_carton(player, carton) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int is_bingo_in_carton(struct Player *player, int carton)
{
    for (int number_position = 0; number_position < BINGO; ++number_position)
    {

        if (is_bingo(player, carton, number_position) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int is_bingo(struct Player *player, int carton, int number_position)
{
    if (current_number == player->cartones[carton][number_position][NUMBERS_CARTON])
    {
        if (player->cartones[carton][number_position][BINGO_CARTON] == 1)
        {
            return 0;
        }

        ++player->cartones_winner_count[carton];
        player->cartones[carton][number_position][BINGO_CARTON] = 1;

        if (player->cartones_winner_count[carton] == BINGO)
        {
            // printf("player %d, carton %d, cantidad de winners %d\n", player->id, carton, player->cartones_winner_count[carton]);
            // checkout_player_carton(player, carton);
            return 1;
        }
    }
    return 0;
}

void host_function(int number_vector[BINGO_MAX_NUMBER])
{
    // time_t t = i;
    // srand((unsigned)time(&t));

    // hasta que alguien gane
    for (winner; winner == 0; winner)
    {
        int new_number = number_vector[current_number_count];
        // int new_number = get_random_number(1, BINGO_MAX_NUMBER);

        printf("el host tiene un numero nuevo, esperando a que todos los jugadores esten listos\n");
        // wait ( que todos los players tengan cartones llenos)
        // wait( que todos hayan buscado el numero en sus cartones)
        WaitForSingleObject(host_semaphore, INFINITE);
        WaitForSingleObject(player_semaphore, INFINITE);

        if (winner != 0)
        {
            printf("\n\nBINGO BY PLAYER %d\n\n", winner);
        }
        else
        {
            // asignar nuevo numero
            printf("el nuevo numero es %d\n", new_number);
            current_number = new_number;
            current_number_count++;

            // signal( nuevo numero)
            num_players_ready = 0;
            ReleaseSemaphore(player_semaphore, 1, NULL);
        }
    }

    // reinicia juego

    printf("host cierra el bingo en el numero %d\n", current_number_count);
    ReleaseSemaphore(player_semaphore, 1, NULL);
}

int get_random_number(int from, int to)
{
    int delta = 100;
    if (from > to)
        return -1;

    int new_number = rand() % delta;
    // validacion
    // no mayor que to
    // y
    // no menor que from
    for (new_number; new_number > to || new_number < from; new_number = rand() % delta)
        ;

    return new_number;
}

int get_random_number_from_1_to_10()
{
    int new_number = rand() % 10;

    if (new_number == 0)
    {
        new_number = 10;
    }

    return new_number;
}

void checkout_player_bingo(struct Player *p)
{
    printf("player %d con %d cartones\n\n", p->id, p->n_cartones);
    for (int i = 0; i < p->n_cartones; ++i)
    {
        for (int j = 0; j < BINGO; ++j)
        {
            printf("carton %d, espacio %d, valor %d \n", i, j, p->cartones[i][j][NUMBERS_CARTON]);
            printf("carton %d, espacio %d, status %d \n", i, j, p->cartones[i][j][BINGO_CARTON]);
        }
        printf("carton %d, winners %d\n\n", i, p->cartones_winner_count[i]);
    }
}

void checkout_player_carton(struct Player *p, int carton)
{
    for (int i = 0; i < BINGO; ++i)
    {
        printf("player %d, carton %d, espacio %d, valor %d \n", p->id, carton, i, p->cartones[carton][i][NUMBERS_CARTON]);
        printf("player %d, carton %d, espacio %d, status %d \n", p->id, carton, i, p->cartones[carton][i][BINGO_CARTON]);
    }
    printf("player %d, carton %d, winners %d\n\n", p->id, carton, p->cartones_winner_count[carton]);
}

int number_not_in_vector(int vector[], int size, int number)
{
    for (int i = 0; i < size; ++i)
    {
        if (vector[i] == number)
        {
            return 0;
        }
    }
    return 1;
}

void get_random_number_vector(int number_vector[BINGO_MAX_NUMBER])
{
    int tries = 0;
    for (int i = 0; i < BINGO_MAX_NUMBER; ++i)
    {
        tries++;
        if (tries > 300)
        {
            printf("%d intentos de numeros random, rellenando vector...\n", tries);
            break;
        }

        int new_number = get_random_number(1, BINGO_MAX_NUMBER);

        // si el numero esta en el vector reinicia el ciclo actual
        if (number_not_in_vector(number_vector, BINGO_MAX_NUMBER, new_number) != 1)
        {
            --i;
            continue;
        }

        number_vector[i] = new_number;
    }

    if (tries > 300)
    {
        for (int i = 1; i <= BINGO_MAX_NUMBER; ++i)
        {
            int new_number = i;
            if (number_not_in_vector(number_vector, BINGO_MAX_NUMBER, new_number) != 1)
            {
                continue;
            }else{
                for( int j = 0; j < BINGO_MAX_NUMBER; j++ ){
                    if(number_vector[j] == 0){
                        number_vector[j] = new_number;
                    }
                }
            }
        }
    }
}

void reset_random_number_vector(int number_vector[BINGO_MAX_NUMBER])
{
    for (int i = 0; i < BINGO_MAX_NUMBER; ++i)
    {
        number_vector[i] = 0;
    }
}

void init_players(struct Player *players[NUM_PLAYERS])
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        // nuevo player
        //  struct Player new_player;
        players[i] = (struct Player *)malloc(sizeof(struct Player));
        players[i]->id = i + 1;
        players[i]->cartones_ganadores = 0;
        players[i]->cartera = 100;
    }
}

void prepare_players(struct Player *players[NUM_PLAYERS])
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        empty_player_cartones(players[i]);

        players[i]->current_number_count = 0;
        
        players[i]->n_cartones = get_random_number_from_1_to_10();

        //validacion de que el jugador puede comprar los cartones
        int deduction = players[i]->n_cartones * 5;
        if( players[i]->cartera - deduction < 0){

            
            if(players[i]->cartera < 5){
                //no juega
                players[i]->n_cartones = 0;
                continue;

            //si puede jugar con al menos 1, juega con ese     
            }else{
                players[i]->n_cartones = 1;
                
            }

        }

        fill_player_cartones(players[i]);
    }
}

void empty_player_cartones(struct Player *player)
{
    // llenar cartones
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {
            player->cartones[carton][space][NUMBERS_CARTON] = 0;
        }
    }
}

int get_winner()
{
    return winner;
}