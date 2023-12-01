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
1. Inicializa los semáforos
2. Corre los procesos
3. Cierra los semáforos

_start_locks()_ y _destroy_locks()_ se importan de [lock.h](./ejercicio_1/lock.h)
```c 
//main.c
#include <stdio.h>
#include "lock.h"
#include <windows.h>
```

### Implementación De Los Semáforos
#### lock.c
```c
#include <windows.h>
#include <stdio.h>

HANDLE writer_semaphore;
HANDLE admin_semaphore;
int num_readers = 0;

void start_locks(){
    printf("SET - Initialize the semaphores\n");
    writer_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    admin_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

void destroy_locks(){
    printf("SET - destroy the semaphores\n");
    CloseHandle(writer_semaphore);
    CloseHandle(admin_semaphore);
}
```
 **HANDLE**: Varible utilizada por windows.h. Esta variable puede ser usada para diferentes cosas, 2 de ellas son: semáforos e hilos.
    En este caso declaramos 2 semáforos, admin con 1 recurso y writer ( escritor ) con 1 recurso

* **start_locks/destroy_locks**: Funciones encargadas de inicializar y destruir los semáforos respectivamente.

```c
void reader_lock()
{
    num_readers++;
    console_log(1, "READER - new reader, current number of readers: %d\n",   num_readers);
    WaitForSingleObject(writer_semaphore, INFINITE);
    WaitForSingleObject(admin_semaphore, INFINITE);
    console_log(1,"READER - CRITICAL SECTION\n");
}

void reader_unlock()
{
    num_readers--;
    console_log(1, "READER - CRITICAL SECTION FIN\n");
    ReleaseSemaphore(admin_semaphore, 1, NULL);
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}
```
* **reader_lock**: Aumenta el contador *num_readers*, coloca en wait los semáforos de escritor y admin y finalmente entra en la sección critica.

* **reader_unlock**: Decrementa el contador *num_readers*, sale de la sección crítica y manda signal a los semáforos de escritor y admin.

```c
void writer_lock()
{
    console_log(1, "WRITER - new writer\n");
    WaitForSingleObject(writer_semaphore, INFINITE);
    WaitForSingleObject(admin_semaphore, INFINITE);
    console_log(1, "WRITER - Critic section\n");
}

void writer_unlock()
{
    console_log(1, "WRITER - Critic section FIN\n");
    ReleaseSemaphore(admin_semaphore, 1, NULL);
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}
```
* **writer_lock**: Coloca en wait los semáforos de escritor y admin y finalmente entra en la sección critica.

* **writer_unlock**: Sale de la sección crítica y manda signal a los semáforos de escritor y admin.
```c
void admin_lock(){
    console_log(1, "ADMIN - new admin\n");
    WaitForSingleObject(admin_semaphore, INFINITE);

}

void admin_unlock(){
    console_log(1 ,"ADMIN - CRITICAL SECTION FIN\n");
    ReleaseSemaphore(admin_semaphore, 1, NULL);
}
```
* **admin_lock**: Coloca en wait el semáforo del admin y entra a la sección crítica

* **admin_unlock**: Sale de la sección crítica y manda signal al semáforo del admin

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
void show_horas_pico()
{
    char * opcional;
    printf("Presione ENTER para continuar con las horas pico.");
    scanf("%c", &opcional);
    printf("5. Horas pico por dia\n");
    for (int day = 0; day < MAX_DAYS; ++day)
    {
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
```
* **show_horas_pico**: Imprime en pantalla los resultados de las horas pico por día.
* **show_counter_by_type_and_day_and_hour**: Imprime en pantalla los resultados por tipo, día y hora.
* **show_counters**: Imprime en pantalla los resultados de las funciones previamente comentadas.
* **show_counter_by_type_and_day**: Imprime en pantalla los resultados por tipo y día.

### Implementación Del Logger

#### logger.c

```c
int hide_logs = 0;
int use_file = 0;
int skip_logs = 0;
```

* **hide_logs**: Variable encargada de determinar si saltar los registros (se necesita que la variable *hiddeable se encuentre en 1*, ya que estas trabajan en conjunto).
* **use_file**: Variable encargada de determinar si el log se escribirá en un archivo.
* **skip_logs**: Variable encargada de determinar si saltar los registros.

```c
void console_log(  int hiddeable, char* format, ... ) {
    if(skip_logs){
        return;
    }

    if(hiddeable && hide_logs){
        return;
    }

    va_list args;
    va_start( args, format );

    if(use_file){
        FILE *f;
        f = fopen("1.log", "a+");
        if (f == NULL) { 
            /* Something is wrong   */
            return;
        }
        vfprintf(f, format, args);

        fclose(f);

    }else{
        vprintf( format, args );

    }

    va_end( args );

}
```
* **console_log**: Función encargada de imprimir en pantalla los registros. En caso tal de que la variable *skip_logs* ó *hiddeable* en conjunto con *hide_logs*, ningún registro se mostrará.
    En el caso de que la variable *use_file* se encuntre en verdadero, se creará y escribirá dentro del archivo 1.log los mensajes con sus correspondientes argumentos. Caso contrario, se imprimirán en pantalla
    
```c
int set_variable( int status, int *variable ){
    if(status > 1 || status < 0){
        return 1;
    }

    *variable = status;
}

int set_hide_logs(int status){
    set_variable(status, &hide_logs);
}

int set_use_file(int status){
    set_variable(status, &use_file);
}

int set_skip_logs( int status){
    set_variable(status, &skip_logs);
}
```

* **set_variable**: Función encargada de asignar un estado ingresando a una variable y el estado el cual se desea asignar a la variable

* **set_hide_logs/set_use_file/set_skip_logs**: Funciones encargadas de modificar el estado de las variables *hide_logs*, *use_file* y *skip_logs* respectivamente

