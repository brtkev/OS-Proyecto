#include <stdio.h>
#include "counter.h"

void count_up(int type, int hour, int day)
{

    if (counter[day][hour][type])
    {
        ++counter[day][hour][type];
    }
    else
    {
        counter[day][hour][type] = 1;
    }
}

int get_count(int type, int hour, int day)
{
    return counter[day][hour][type];
}

// todas las operaciones
int get_full_count()
{
    int accumulator = 0;
    for (int day = 0; day < MAX_DAYS; ++day)
    {
        for (int hour = 0; hour < MAX_HOURS; ++hour)
        {
            for (int type = 0; type < TYPES; ++type)
            {
                accumulator += counter[day][hour][type];
            }
        }
    }
    return accumulator;
}

// todas las operaciones por hora ( desde 0 hasta 23 horas)
int get_full_count_by_hour(int hour)
{
    int accumulator = 0;

    for (int day = 0; day < MAX_DAYS; ++day)
    {
        for (int type = 0; type < TYPES; ++type)
        {
            accumulator += counter[day][hour][type];
        }
    }

    return accumulator;
}

// todas las operaciones por tipo ( Reader, Writer, Admin)
int get_full_count_by_type(int type)
{
    int accumulator = 0;

    for (int day = 0; day < MAX_DAYS; ++day)
    {
        for (int hour = 0; hour < MAX_HOURS; ++hour)
        {
            accumulator += counter[day][hour][type];
        }
    }

    return accumulator;
}

// todas las operaciones por tipo y dia
int get_count_by_type_and_day(int type, int day)
{
    int accumulator = 0;

    for (int hour = 0; hour < MAX_HOURS; ++hour)
    {
        accumulator += counter[day][hour][type];
    }

    return accumulator;
}

void show_horas_pico()
{
    char * opcional;
    printf("Presione ENTER para continuar con las horas pico.");
    scanf("%c", &opcional);
    printf("5. Horas pico por dia\n");
    for (int day = 0; day < MAX_DAYS; ++day)
    {
        if(day != 0){
            printf("Presione ENTER para continuar.");
            scanf("%c", &opcional);
        }
        printf("    dia %d\n", day + 1);

        printf("        tipo lector:\n");
        show_counter_by_type_and_day_and_hour(day, READER_TYPE);

        printf("        tipo escritor:\n");
        show_counter_by_type_and_day_and_hour(day, WRITER_TYPE);

        printf("        tipo admin:\n");
        show_counter_by_type_and_day_and_hour(day, ADMIN_TYPE);
    }
}

void show_counter_by_type_and_day_and_hour(int day, int type)
{
    int sum = get_count_by_type_and_day(type, day);
    int media = sum / MAX_DAYS;
    for (int hour = 0; hour < MAX_HOURS; ++hour)
    {
        printf("            hora %d con %d operaciones.\n", hour + 1, counter[day][hour][type]);
        if (counter[day][hour][type] > media * 2)
        {
            printf("                hora %d es una hora pico\n\n", hour + 1);
        }
    }
}

// interface
void show_counters()
{
    printf("1. numero total de operaciones : %d\n", get_full_count());

    printf("2. numero de operaciones por tipo\n");
    printf("    operaciones por reader: %d\n", get_full_count_by_type(READER_TYPE));
    printf("    operaciones por writer: %d\n", get_full_count_by_type(WRITER_TYPE));
    printf("    operaciones por admin: %d\n", get_full_count_by_type(ADMIN_TYPE));

    printf("3. numero de operaciones por hora: \n");
    for (int hour = 0; hour < MAX_HOURS; ++hour)
    {
        printf("    Hora %d:00, operaciones: %d\n", hour + 1, get_full_count_by_hour(hour));
    }

    show_counter_by_type_and_day();

    show_horas_pico();
}

void show_counter_by_type_and_day()
{
    char * opcional;
    printf("Presione ENTER para continuar con las operaciones por tipo y por dia.");
    scanf("%c",  &opcional);
    printf("4. numero total de operaciones por tipo y por dia\n");
    for (int day = 0; day < MAX_DAYS; ++day)
    {
        int reader = get_count_by_type_and_day(READER_TYPE, day),
        writer = get_count_by_type_and_day(WRITER_TYPE, day),
        admin = get_count_by_type_and_day(ADMIN_TYPE, day);

        printf("    dia %d: %d operaciones\n", day + 1, reader + writer + admin);
        printf("        operaciones por reader: %d\n", reader);
        printf("        operaciones por writer: %d\n", writer);
        printf("        operaciones por admin: %d\n", admin);
    }
}