#include <windows.h>
#include "lock.h"

// Create two semaphores: one for admins and one for writers.
HANDLE host_semaphore;
HANDLE player_semaphore;
HANDLE bingo_semaphore;

int num_players_not_ready = NUM_PLAYERS;
int winner = 0;
int current_number;

void start_locks()
{
    // Initialize the semaphores.
    printf("SET - Initialize the semaphores\n");
    host_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
    player_semaphore = CreateSemaphore(NULL, 0, NUM_PLAYERS, NULL);
    bingo_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

void destroy_locks()
{
    // Destroy the semaphores
    printf("SET - destroy the semaphores\n");
    CloseHandle(host_semaphore);
    CloseHandle(player_semaphore);
}

struct Player
{
    /* data */
    int id, score,
        n_cartones;
    int cartones[MAX_CARTONES][BINGO][2];
    int cartones_winner_count[BINGO];
};

void player_function(DWORD i)
{
    // init player
    struct Player player;
    player.id = i;
    player.score = 0;
    player.n_cartones = (rand() % 10);

    // llenar cartones
    for (int carton = 0; carton < player.n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {

            int number = (rand() % 100);

            // validacion no mayor que 90
            for (number; number > 90; number = (rand() % 100))
                ;

            player.cartones[carton][space][NUMBERS_CARTON] = number;
            player.cartones[carton][space][BINGO_CARTON] = 0;
        }
        player.cartones_winner_count[carton] = 0;
    }

    for (winner; winner == 0; winner)
    {
        // signal( al gordo )
        --num_players_not_ready;
        if (num_players_not_ready == 0)
        {
            ReleaseSemaphore(host_semaphore, 1, NULL);
        }

        // wait ( por el gordo que diga numero)
        WaitForSingleObject(player_semaphore, INFINITE);

        ++num_players_not_ready;
        // BINGO
        if (is_bingo_in_player(player))
        {

            // wait (esperan otros jugadores)
            WaitForSingleObject(bingo_semaphore, INFINITE);
            //  si nadie ha avisado
            if (winner == 0)
            {
                // avisa al gordo que gana
                winner = player.id;
            }
            // signal( libera otros jugadores)
            ReleaseSemaphore(bingo_semaphore, 1, NULL);
        }

        // signal ( cuando reviso si tiene numero )
    }
}

int is_bingo_in_player(struct Player player)
{
    for (int carton = 0; carton < player.n_cartones; ++carton)
    {
        if(is_bingo_in_carton(player, carton))  return 1;
    }
}

int is_bingo_in_carton(struct Player player, int carton)
{
    for (int number_position = 0; number_position < BINGO; ++number_position)
    {

        if(is_bingo(player, carton, number_position)) return 1;
    }
}

int is_bingo(struct Player player, int carton, int number_position){
    if (current_number == player.cartones[carton][number_position][NUMBERS_CARTON])
        {
            if (player.cartones[carton][number_position][BINGO_CARTON] == 1) return 0;

            ++player.cartones_winner_count[carton];
            if (player.cartones_winner_count[carton] == BINGO)
                return 1;

            player.cartones[carton][number_position][BINGO_CARTON] = 1;

            return 0;
        }
}

void host_function(DWORD i)
{

    // hasta que alguien gane
    for (winner; winner == 0; winner)
    {
        int new_number = (rand() % 100);
        // validacion no mayor que 90
        for (new_number; new_number > 90; new_number = (rand() % 100))
            ;

        // wait ( que todos los players tengan cartones llenos)
        WaitForSingleObject(host_semaphore, INFINITE);

        // asignar nuevo numero
        current_number = new_number;

        // signal( nuevo numero)
        // solo hace signal si tiene minimo NUM_PLAYERS espacios
        for (; !ReleaseSemaphore(player_semaphore, NUM_PLAYERS, NULL);)
            ;

        // wait( que todos hayan buscado el numero en sus cartones)
    }

    // envia plata a la cartera del jugador ganador

    // reinicia juego

    printf("host function %d\n", i);
}