#pragma once

#define MAX_DAYS 30
#define MAX_HOURS 24
#define TYPES 3
#define READER_TYPE 0
#define WRITER_TYPE 1
#define ADMIN_TYPE 2

int counter[MAX_DAYS][MAX_HOURS][TYPES];

void count_up(int type, int hour, int day);
int get_count(int type, int hour, int day);
int get_full_count();
int get_full_count_by_hour(int hour);
int get_full_count_by_type(int type);
int get_count_by_type_and_day(int type, int day);
void show_counters();
void show_horas_pico();
void show_counter_by_type_and_day_and_hour(int day, int type);
void show_counter_by_type_and_day();