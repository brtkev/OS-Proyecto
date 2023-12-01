#pragma once
#include <windows.h>
#define NUM_PLAYERS 6
#define MAX_CARTONES 10
#define BINGO 15
#define NUMBERS_CARTON 0
#define BINGO_CARTON 1
#define BINGO_MAX_NUMBER 90


struct Player
{
    /* data */
    int id, score,
        n_cartones;
    int cartones[MAX_CARTONES][BINGO][2];
    int cartones_winner_count[MAX_CARTONES];
};

void start_locks();
void destroy_locks();

int get_random_number(int from, int to);
int get_random_number_from_1_to_10();
void player_function(struct Player * player);
void host_function(DWORD i);
void fill_player_cartones(struct Player * player);
int is_bingo_in_player(struct Player player);
int is_bingo_in_carton(struct Player player, int carton);
int is_bingo(struct Player player, int carton, int number_position);                    
void checkout_player_bingo(struct Player * p);