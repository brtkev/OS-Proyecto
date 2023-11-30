# OS-Proyecto
Proyecto Para Sistemas Operativos 2023



# Ejercicio 1

## 1. Make File
Para compilar y ejecutar el ejercicio 1, debemos ejecutar en el terminal los siguientes pasos:


```shell
    make 1
    ./1
```

### Explicación

dentro del archivo [Makefile](./Makefile) encontraremos 2 funciones

```make
EJ_1_FILES= ejercicio_1/*.c
...
1: 1.exe
	echo "make 1.exe"

1.exe: $(EJ_1_FILES)
	gcc $(CFLAG) -o $@ $(EJ_1_FILES)
```

la funcion "1", llama a la funcion "1.exe" y luego ejecuta el echo

la funcion "1.exe", llama a EJ_1_FILES, los cuales son todos los archivos .c del directorio [ejercicio 1](./ejercicio_1), con estos objetos dentro de la funcion, luego se compilan usando el comando gcc con la bandera -o y se usa el nombre de la funcion (1.exe) utilizando $@

## 2. Ejecución

Primero se ejecuta _main()_ dentro [main.c](./ejercicio_1/main.c)

esta funcion hace 3 cosas:
```c
//main.c
// Initialize the semaphores.
start_locks();

process_runner();

// Destroy the semaphores
destroy_locks();
```
1. inicializa los semaforos
2. corre los procesos
3. cierra los semaforos

_start_locks()_ y _destroy_locks()_ se importan de [lock.h](./ejercicio_1/lock.h)
```c 
//main.c
#include <stdio.h>
#include "lock.h"
#include <windows.h>
```

### Implementación De Los Semáforos
```c
//lock.c
#include <windows.h>
#include <stdio.h>

// Create two semaphores: one for admins and one for writers.
HANDLE writer_semaphore;
HANDLE admin_semaphore;
int num_readers = 0;

void start_locks(){
    // Initialize the semaphores.
    printf("SET - Initialize the semaphores\n");
    writer_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    admin_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

void destroy_locks(){
    // Destroy the semaphores
    printf("SET - destroy the semaphores\n");
    CloseHandle(writer_semaphore);
    CloseHandle(admin_semaphore);
}
```
HANDLE es una varible utilizada por windows.h, esta variable puede ser usada para diferentes cosas, 2 de ellas son: semaforos e hilos.

En este caso declaramos 2 semaforos, admin con 1 recurso y writer ( escritor ) con 1 recurso 

### Implementación De Los Contadores

#### counter.h
```c
#define MAX_DAYS 30
#define MAX_HOURS 24
#define TYPES 3
#define READER_TYPE 0
#define WRITER_TYPE 1
#define ADMIN_TYPE 2

int counter[MAX_DAYS][MAX_HOURS][TYPES];
```
* **MAX_DAYS**: Constante. Número de días del mes.
* **MAX_HOURS**: Constante. Número de horas del día.
* **TYPES**: Constante. Número de tipos de operaciones que existen.
    * **READER_TYPE**: Número asignado al tipo de operación "lector"
    * **WRITER_TYPE**: Número asignado al tipo de operación "escritor"
    * **ADMIN_TYPE**: Número asignado al tipo de operación "administrador"
* **counter**: Matríz de contadores.

#### counter.c
```c
void count_up(int type, int hour, int day){

    if(counter[day][hour][type]){
        ++counter[day][hour][type];

    }else{
        counter[day][hour][type] = 1;
    }
}
```
* **count_up**: Esta función le suma 1 al tipo en la posición indicada en caso tal de que exista. Caso contrario (0 o undefined), se le asigna el valor de 1

```c
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
```
* **get_count**: Retorna el valor actual de los datos indicados.
* **get_full_count**: Retorna todas las operaciones.
* **get_full_count_by_hour**: Retorna todas las operaciones por hora.
* **get_full_count_by_type**: Retorna todas las operaciones por tipo.
* **get_count_by_type_and_day**: Retorna la cuenta de cada tipo y cada día.

```c
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
```
* **show_counters**: Imprime en pantalla los resultados de las funciones previamente comentadas.

### implementacion de el logger
https://www.ozzu.com/wiki/504927/writing-a-custom-printf-wrapper-function-in-c
