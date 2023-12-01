#pragma once
#include <windows.h>
#define NUM_PLAYERS 6
#define MAX_CARTONES 10
#define BINGO 15
#define NUMBERS_CARTON 0
#define BINGO_CARTON 1

void start_locks();
void destroy_locks();

void player_function(DWORD i);
void host_function(DWORD i);
