#pragma once
#include <windows.h>
#define NUM_PLAYERS 7
#define MAX_CARTONES 10
#define BINGO 15
#define NUMBERS_CARTON 0
#define BINGO_CARTON 1
#define BINGO_MAX_NUMBER 90


struct Player
{
    /* data */
    int id, 
        cartones_ganadores,
        n_cartones, 
        current_number_count;
    int cartones[MAX_CARTONES][BINGO][2];
    int cartones_winner_count[MAX_CARTONES];

    float cartera;
};


void start_locks();
void destroy_locks();
void set_current_players(int n);
int get_random_number(int from, int to);
int get_random_number_from_1_to_10();
void player_function(struct Player *player_ptr);
void host_function(int number_vector [BINGO_MAX_NUMBER]);
void fill_player_cartones(struct Player *player);
int is_bingo_in_player(struct Player * player);
int is_bingo_in_carton(struct Player * player, int carton);
int is_bingo(struct Player * player, int carton, int number_position);                    
void checkout_player_bingo(struct Player * p);
void checkout_player_carton(struct Player *p, int carton);
int number_not_in_vector(int vector[], int size, int number);
void get_random_number_vector( int number_vector [BINGO_MAX_NUMBER]);
void reset_random_number_vector(int number_vector [BINGO_MAX_NUMBER]);
void init_players(struct Player *players[NUM_PLAYERS]);
void prepare_players(struct Player *players[NUM_PLAYERS]);
void empty_player_cartones(struct Player *player);
int get_winner();