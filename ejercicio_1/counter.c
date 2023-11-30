#include <stdio.h>
#include "counter.h"


void count_up(int type, int hour, int day){

    if(counter[day][hour][type]){
        ++counter[day][hour][type];

    }else{
        counter[day][hour][type] = 1;
    }
}

int get_count(int type, int hour, int day){
    return counter[day][hour][type];
}

int get_full_count(){
    int accumulator = 0;
    for(int day = 0; day < MAX_DAYS; ++day){
        for( int hour = 0; hour < MAX_HOURS; ++hour){
            for( int type = 0; type < TYPES; ++type){
                accumulator += counter[day][hour][type];
            }
        }
    }
    return accumulator;
}

int get_full_count_by_hour(int hour){
    int accumulator = 0;

    for(int day = 0; day < MAX_DAYS; ++day){
        for( int type = 0; type < TYPES; ++type){
            accumulator += counter[day][hour][type];
        }
    }

    return accumulator;
}

int get_full_count_by_type(int type){
    int accumulator = 0;

    for(int day = 0; day < MAX_DAYS; ++day){
        for( int hour = 0; hour < MAX_HOURS; ++hour){
            accumulator += counter[day][hour][type];
        }
    }

    return accumulator;
}

int get_count_by_type_and_day(int type, int day){
    int accumulator = 0;

    for( int hour = 0; hour < MAX_HOURS; ++hour){
        accumulator += counter[day][hour][type];
    }

    return accumulator;
}


void show_counters(){
    printf("1. numero total de operaciones : %d\n", get_full_count());

    printf("2. numero de operaciones por tipo\n");
    printf("    operaciones por reader: %d\n", get_full_count_by_type(READER_TYPE));
    printf("    operaciones por writer: %d\n", get_full_count_by_type(WRITER_TYPE));
    printf("    operaciones por admin: %d\n", get_full_count_by_type(ADMIN_TYPE));

    printf("3. numero de operaciones por dia: \n");
    for( int hour = 0; hour < MAX_HOURS; ++hour){
        printf("    Hora %d:00, operaciones: %d\n", hour+1, get_full_count_by_hour(hour));
    }

    printf("4. numero total de operaciones por tipo, por dia\n");
    for( int day = 0; day < MAX_DAYS; ++day){
        printf("    dia %d:\n", day);
        printf("        operaciones por reader: %d\n", get_count_by_type_and_day(READER_TYPE, day));
        printf("        operaciones por writer: %d\n", get_count_by_type_and_day(WRITER_TYPE, day));
        printf("        operaciones por admin: %d\n", get_count_by_type_and_day(ADMIN_TYPE, day));
    }

}