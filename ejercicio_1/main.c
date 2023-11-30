#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "lock.h"
#include "counter.h"
#include "logger.h"

#define THREAD_COUNT 17

DWORD WINAPI reader_function(DWORD i)
{
    
    reader_lock();
    console_log(0, "CRITIAL SECTION - Reader Function\n");
    reader_unlock();
    
}

DWORD WINAPI writer_function(DWORD i)
{
    
    writer_lock();
    console_log(0 ,"CRITIAL SECTION - writer Function\n");
    writer_unlock();
    
}

DWORD WINAPI admin_function(DWORD i)
{
    
    admin_lock();
    console_log(0 ,"CRITIAL SECTION - admin Function\n");
    admin_unlock();
}

void process_runner(int hour, int day)
{
    HANDLE threads[THREAD_COUNT];
    DWORD i;

    for (i = 0; i < THREAD_COUNT; i++)
    {
        DWORD thread_id;
        if (i % 3 == 0)
        {

            count_up(ADMIN_TYPE, hour, day);
            threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)admin_function, (PVOID)i, 0, &thread_id);
        }
        else if (i % 2 == 0)
        {

            count_up(WRITER_TYPE, hour, day);
            threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer_function, (PVOID)i, 0, &thread_id);
        }
        else
        {
            count_up(READER_TYPE, hour, day);
            threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader_function, (PVOID)i, 0, &thread_id);
        }

        if (threads[i] == NULL)

        {
            console_log(0 ,"LCreateThread() failed, error %u\n", GetLastError());

            ExitProcess(1);

            return;
        }
        // else printf("LCreateThread() is OK, thread ID %u\n", thread_id);
    }
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
}

void hour_runner(int day)
{
    for (int hour = 0; hour < MAX_HOURS; ++hour)
    {
        console_log(0, "\nSTART - hour %d\n\n", hour + 1);
        process_runner(hour, day);
    }
}

void daily_runner()
{
    for (int day = 0; day < MAX_DAYS; ++day)
    {
        console_log(0 ,"\nSTART - day %d\n\n", day + 1);
        hour_runner(day);
    }
}



int main(int argc, int *argv[])
{
    printf("You have entered %d arguments:\n", argc - 1);

    printf("opciones validas (0 o 1, todas estan en 0 por defecto).\n");
    printf("Argumento 1, skip logs\nSalta todos los pasos de los hilos y muestra el resultado directamente.\n");
    printf("Argumento 2, hide logs\nOculta cada paso de los hilos, solo muestra las Secciones Criticas.\n");
    printf("Argumento 3, use file\nEn vez de mostrar el proceso de los hilos por consola, los guarda en un archivo '1.log'.\n\n");

    char *output; // es necesario pasarlo como argumento, pero no se usa
    long argument;

    for (int i = 0; i < argc; i++) {
        if( i == 1){
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 1: %d\n", argument);        
            set_skip_logs(argument);

        }else if( i == 2 ){
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 2: %d\n", argument);        
            set_hide_logs(argument);

        }else if( i == 3 ){
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 3: %d\n", argument);        
            set_use_file(argument);
        }
        // printf("%s\n", argv[i]);
    }

    printf("\nPresione Enter para empezar.");
    scanf("%c", &output);
    
    // Initialize the semaphores.
    start_locks();
    
    //corre todo
    daily_runner();

    // Destroy the semaphores
    destroy_locks();

    show_counters();

    return 0;
}
