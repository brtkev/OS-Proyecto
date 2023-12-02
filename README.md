# OS-Proyecto
Proyecto Para Sistemas Operativos 2023



# Ejercicio 1

## 1. Make File
Para compilar y ejecutar el ejercicio 1, debemos ejecutar en el terminal los siguientes pasos:


```shell
    make 1
    ./1
```

## 2. Ejecución

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
    ++num_admins;
    console_log(1, "ADMIN - new admin\n");
    WaitForSingleObject(admin_semaphore, INFINITE);

}

void admin_unlock(){
    --num_admins;
    console_log(1 ,"ADMIN - CRITICAL SECTION FIN\n");
    ReleaseSemaphore(admin_semaphore, 1, NULL);
}
```
* **admin_lock**: Aumenta el contador *num_admins*, coloca en wait el semáforo del admin y entra a la sección crítica

* **admin_unlock**: Decrementa el contador *num_admins*, sale de la sección crítica y manda signal al semáforo del admin

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
* **get_count_by_type_and_day**: Retorna todas las operaciones por cada tipo y cada día.

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
* **show_horas_pico**: Interfaz para mostrar los resultados por tipo, día, hora y las horas pico.
* **show_counter_by_type_and_day_and_hour**: Imprime en pantalla los resultados por tipo, día, hora y las horas pico.
* **show_counters**: Imprime en pantalla todos los tipos de operaciones.
* **show_counter_by_type_and_day**: Imprime en pantalla los resultados por tipo y día.

### Implementación Del Logger

#### logger.c

```c
int hide_logs = 0;
int use_file = 0;
int skip_logs = 0;
```

* **hide_logs**: Variable encargada de determinar si saltar la impresión en pantalla ciertos hilos (se necesita que la variable *hiddeable* se encuentre en 1, ya que estas trabajan en conjunto. Solo se muestran las secciones críticas de los hilos).
* **use_file**: Variable encargada de determinar si el log se escribirá en un archivo.
* **skip_logs**: Variable encargada de determinar si saltar la impresión en pantalla de los hilos.

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

### Ejecución de main.c
#### main.c

```c
int thread_count;
DWORD WINAPI reader_function(DWORD i)
{

    reader_lock();
    console_log(0, "CRITIAL SECTION - Reader Function\n");
    reader_unlock();
}

DWORD WINAPI writer_function(DWORD i)
{

    writer_lock();
    console_log(0, "CRITIAL SECTION - writer Function\n");
    writer_unlock();
}

DWORD WINAPI admin_function(DWORD i)
{

    admin_lock();
    console_log(0, "CRITIAL SECTION - admin Function\n");
    admin_unlock();
}
```
* **reader_function/writer_function/admin_function**: Cada una de estas funciones ejecuta su respectivo *lock*, un *console_log* para indicar que está en su sección crítica y finalmente se ejecuta su *unlock*.

```c
void process_runner(int hour, int day)
{
    int thread_count = rand() % 10;
    HANDLE threads[thread_count];
    DWORD i;

    for (i = 0; i < thread_count; i++)
    {
        int thread_type = rand() % 10;
        DWORD thread_id;
        if (thread_type % 3 == 0)
        {

            count_up(ADMIN_TYPE, hour, day);
            threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)admin_function, (PVOID)i, 0, &thread_id);
        }
        else if (thread_type % 2 == 0)
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
            console_log(0, "LCreateThread() failed, error %u\n", GetLastError());

            ExitProcess(1);

            return;
        }
    }
    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
}
```

* **process_runner**: Esta función comienza asignándole un número aleatorio a *thread_count*, nuestra variable para determinar el tamaño de nuestro arreglo de hilos en la variable *threads* y un iterador *i*.
\
    Adicionalmente se define otro número aleatorio a la variable *thread_type* y definimos una variable thread_id para ser pasada como un apuntador una vez vayamos creando cada uno de los hilos.
    \
    Los siguientes condicionales determinan si los hilos a crearse son para los casos de *admin* , *writer* o *reader*. En caso tal de que en nuestro arreglo de hilos exista una posición en la cual se encuentre en **NULL** se imprime el error. Finalmente esperamos a que los procesos correspondientes terminen.

```c
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
        console_log(0, "\nSTART - day %d\n\n", day + 1);
        hour_runner(day);
    }
}
```

* **hour_runner**: Función la cual iniciará los procesos por hora de cada uno de los días

* **daily_runner**: Funcion la cual simulará el inicio cada uno de los días.

```c
int main(int argc, int *argv[])
{
    time_t t1;
    srand ( (unsigned) time (&t1));
    
    printf("You have entered %d arguments:\n", argc - 1);

    printf("opciones validas (0 o 1, todas estan en 0 por defecto).\n");
    printf("Argumento 1, skip logs\nSalta todos los pasos de los hilos y muestra el resultado directamente.\n");
    printf("Argumento 2, hide logs\nOculta cada paso de los hilos, solo muestra las Secciones Criticas.\n");
    printf("Argumento 3, use file\nEn vez de mostrar el proceso de los hilos por consola, los guarda en un archivo '1.log'.\n\n");

    char *output;
    long argument;

    for (int i = 0; i < argc; i++)
    {
        if (i == 1)
        {
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 1: %d\n", argument);
            set_skip_logs(argument);
        }
        else if (i == 2)
        {
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 2: %d\n", argument);
            set_hide_logs(argument);
        }
        else if (i == 3)
        {
            argument = strtol((char *)argv[i], &output, 10);
            printf("argumento 3: %d\n", argument);
            set_use_file(argument);
        }
    }

    printf("\nPresione Enter para empezar.");
    scanf("%c", &output);

    start_locks();

    daily_runner();

    destroy_locks();

    show_counters();

    return 0;
}
```

* **main**: Primeramente iniciamos la semilla para los números aleatorios. Posteriormente usamos la función *strtol* para tomar y convertir nuestro input a un tipo de dato *long*. Estos se imprimirán en pantalla dependiendo de cuántos argumentos hayamos escrito.
\
Ulteriormente se pide que el usuario presione enter para ejecutar las funciones *start_locks* y *daily_runner*. Una vez terminadas se ejecutaría *destroy_locks* y se mostraría en pantalla los contadores con la función *show_counters*

# Ejercicio 2

## 1. Make File
Para compilar y ejecutar el ejercicio 2, debemos ejecutar en el terminal los siguientes pasos:


```shell
    make 2
    ./2
```
## 2. Ejecución

### Implementación Del BIngo

#### lock.h
```c
#define NUM_PLAYERS 10
#define MAX_CARTONES 10
#define BINGO 15
#define NUMBERS_CARTON 0
#define BINGO_CARTON 1
#define BINñGO_MAX_NUMBER 90


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
```
* **NUM_PLAYERS**: Tipo número máximo de jugadores.
* **MAX_CARTONES**: Tipo número máximo de cartones
* **BINGO**: Tipo número de valores por cartón
* **NUMBERS_CARTON**: Cartón de números.
* **BINGO_CARTON**: Cartón de bingo (donde se tachará en caso tal de hacer match con lo que diga el host)
* **BINGO_MAX_NUMBER**: Rango de los valores que puede tomar cada casilla del cartón de números (empieza desde 1)
* **Player**: Tipo de dato jugador.
    * **id**: Identificador del jugador
    * **cartones_ganadores**: Cantidad de cartones ganadores.
    * **n_cartones**: Cantidad de cartones que posee en una partida.
    * **current_number_count**: Cantidad de números que ha dicho el host y el jugador ha anotado.
    * **cartones**: La cantidad de cartones x los 15 dijitos que trae un bingo x los dos tipos de cartones que estamos manejando (número y bingo).
    * **cartones_winner_count**: La cantidad de números anotados que lleva en cada cartón. Por eso tiene el mismo tamaño que MAX_CARTONES.
    * **cartera**: Cantidad de fichas que posee el jugador.

#### lock.c
```c
HANDLE host_semaphore;
HANDLE player_semaphore;
HANDLE bingo_semaphore;

int num_players_ready;
int winner;
int current_number;
int current_number_count;
int current_players;
```

* **host_semaphore**: Semáforo para el host.
* **player_semaphore**: Semáforo para los jugadores.
* **bingo_semaphore**: Semáforo para el bingo.
* **num_players_ready**: Número de jugadores listos para jugar.
* **winner**: Bandera ganador. Toma el id del ganador. 0 = no hay ganador aún.
* **current_number**: Número actual que ha dicho el host.
* **current_number_count**: La cantidad de números que ha dicho el host.
* **current_players**: La cantidad actual de jugadores.

```c
void set_current_players(int n){
    current_players = n;
}

void start_locks()
{
    printf("SET - Initialize the semaphores\n");
    host_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
    player_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    bingo_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    num_players_ready = 0;
    current_number_count = 0;
    winner = 0;
}

void destroy_locks()
{
    printf("SET - destroy the semaphores\n");
    CloseHandle(host_semaphore);
    CloseHandle(player_semaphore);
    CloseHandle(bingo_semaphore);
}
```
* **set_current_players**: Asigna la cantidad actual de jugadores.
* **start_locks**: Inicializa los semáforos.
* **destroy_locks**: Destruye los semáforos.

```c
void player_function(struct Player *player_ptr)
{
    if(player_ptr->n_cartones == 0){
        return;
    }
    time_t t = player_ptr->id * 3.1415;
    srand((unsigned)time(&t));

    printf("player id %d\n", player_ptr->id);

    printf("player %d is getting %d cartons\n", player_ptr->id, player_ptr->n_cartones);

    printf("player %d got his %d cartons filled\n", player_ptr->id, player_ptr->n_cartones);
    ++num_players_ready;

    for (winner; winner == 0; winner)
    {
        WaitForSingleObject(player_semaphore, INFINITE);

        if (player_ptr->current_number_count < current_number_count)
        {

            ++player_ptr->current_number_count;

            printf("player %d esta leyendo sus cartones buscando el numero %d\n", player_ptr->id, current_number);

            if (is_bingo_in_player(player_ptr) == 1)
            {
                printf("player %d tiene bingo\n", player_ptr->id);
                WaitForSingleObject(bingo_semaphore, INFINITE);
                if (winner == 0)
                {
                    printf("\n--- player %d avisa que tiene bingo ---\n", player_ptr->id);
                    checkout_player_bingo(player_ptr);
                    player_ptr->cartones_ganadores++;
                    winner = player_ptr->id;
                }
                ReleaseSemaphore(bingo_semaphore, 1, NULL);
            }

            ++num_players_ready;
            printf("player %d esta listo, el numero de jugadores que estan listos es %d\n", player_ptr->id, num_players_ready);
        }

        if (num_players_ready == current_players)
        {
            printf("player %d avisa al host que todos los jugadores estan listos\n", player_ptr->id);
            ReleaseSemaphore(host_semaphore, 1, NULL);
        }
        ReleaseSemaphore(player_semaphore, 1, NULL);
    }

    ReleaseSemaphore(host_semaphore, 1, NULL);
    printf("player %d termino de jugar\n", player_ptr->id);
}
```
* **player_function**: Verifica si el jugador tiene cartones, si tiene cartones, entonces juega. Espera a que los jugadores estén listos para jugar. Espera a que el host libere un número, una vez liberado se lee por todos los jugadores. Si algun jugador tiene **bingo** lo notifica, sino esperan al siguiente número del host.

```c
void fill_player_cartones(struct Player *player)
{
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {
            int new_number = get_random_number(1, BINGO_MAX_NUMBER);

            if (number_not_in_carton(player, carton, new_number) != 1)
            {
                --space;
                continue;
            }

            player->cartones[carton][space][NUMBERS_CARTON] = new_number;
            player->cartones[carton][space][BINGO_CARTON] = 0;
        }
        player->cartones_winner_count[carton] = 0;
    }
}

int number_not_in_carton(struct Player *p, int carton, int number)
{
    for (int i = 0; i < BINGO; ++i)
    {
        if (p->cartones[carton][i][NUMBERS_CARTON] == number)
        {
            return 0;
        }
    }
    return 1;
}
```
* **fill_player_cartones**: Llena todos los cartones de números para cada jugador.
* **number_not_in_carton**: Verifica si el número existe en el cartón.

```c
int is_bingo_in_player(struct Player *player)
{
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        if (is_bingo_in_carton(player, carton) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int is_bingo_in_carton(struct Player *player, int carton)
{
    for (int number_position = 0; number_position < BINGO; ++number_position)
    {

        if (is_bingo(player, carton, number_position) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int is_bingo(struct Player *player, int carton, int number_position)
{
    if (current_number == player->cartones[carton][number_position][NUMBERS_CARTON])
    {
        if (player->cartones[carton][number_position][BINGO_CARTON] == 1)
        {
            return 0;
        }

        ++player->cartones_winner_count[carton];
        player->cartones[carton][number_position][BINGO_CARTON] = 1;

        if (player->cartones_winner_count[carton] == BINGO)
        {
            return 1;
        }
    }
    return 0;
}
```
* **is_bingo_in_player**: Verifica si el jugador tiene bingo.
* **is_bingo_in_carton**: Verifica si el cartón especifico al que está llamando tiene bingo.
* **is_bingo**: Verifica que el cartón actual del jugador actual está consiguiendo un bingo o no.

```c
void host_function(int number_vector[BINGO_MAX_NUMBER])
{
    for (winner; winner == 0; winner)
    {
        int new_number = number_vector[current_number_count];

        printf("el host tiene un numero nuevo, esperando a que todos los jugadores esten listos\n");
        WaitForSingleObject(host_semaphore, INFINITE);
        WaitForSingleObject(player_semaphore, INFINITE);

        if (winner != 0)
        {
            printf("\n\nBINGO BY PLAYER %d\n\n", winner);
        }
        else
        {
            printf("el nuevo numero es %d\n", new_number);
            current_number = new_number;
            current_number_count++;

            num_players_ready = 0;
            ReleaseSemaphore(player_semaphore, 1, NULL);
        }
    }


    printf("host cierra el bingo en el numero %d\n", current_number_count);
    ReleaseSemaphore(player_semaphore, 1, NULL);
}
```
* **host_function**: Espera a que todos los jugadores estén listos. Toma un número del vector de números, asignándolo a *new_number*. En caso tal de que haya un ganador, no dice el siguiente número. Caso contrario, sigue diciendo números mientras todos los jugadores estén listos.

```c
int get_random_number(int from, int to)
{
    int delta = 100;
    if (from > to)
        return -1;

    int new_number = rand() % delta;
    for (new_number; new_number > to || new_number < from; new_number = rand() % delta)
        ;

    return new_number;
}

int get_random_number_from_1_to_10()
{
    int new_number = rand() % 10;

    if (new_number == 0)
    {
        new_number = 10;
    }

    return new_number;
}
```
* **get_random_number**: Retorna un número aleatorio entre *from* (que actúa como mínimo) y *to* (que actúa como un máximo).
* **get_random_number_from_1_to_10**: Retorna un número aleatorio entre el 1 y el 10.

```c
void checkout_player_bingo(struct Player *p)
{
    printf("player %d con %d cartones\n\n", p->id, p->n_cartones);
    for (int i = 0; i < p->n_cartones; ++i)
    {
        for (int j = 0; j < BINGO; ++j)
        {
            printf("carton %d, espacio %d, valor %d \n", i, j, p->cartones[i][j][NUMBERS_CARTON]);
            printf("carton %d, espacio %d, status %d \n", i, j, p->cartones[i][j][BINGO_CARTON]);
        }
        printf("carton %d, winners %d\n\n", i, p->cartones_winner_count[i]);
    }
}

void checkout_player_carton(struct Player *p, int carton)
{
    for (int i = 0; i < BINGO; ++i)
    {
        printf("player %d, carton %d, espacio %d, valor %d \n", p->id, carton, i, p->cartones[carton][i][NUMBERS_CARTON]);
        printf("player %d, carton %d, espacio %d, status %d \n", p->id, carton, i, p->cartones[carton][i][BINGO_CARTON]);
    }
    printf("player %d, carton %d, winners %d\n\n", p->id, carton, p->cartones_winner_count[carton]);
}
```
* **checkout_player_bingo**: Imprime en pantalla todos los cartones del jugador que tiene bingo.
* **checkout_player_carton**: Imprime en pantalla el cartón del jugador a especificar.

```c
int number_not_in_vector(int vector[], int size, int number)
{
    for (int i = 0; i < size; ++i)
    {
        if (vector[i] == number)
        {
            return 0;
        }
    }
    return 1;
}

void get_random_number_vector(int number_vector[BINGO_MAX_NUMBER])
{
    int tries = 0;
    for (int i = 0; i < BINGO_MAX_NUMBER; ++i)
    {
        tries++;
        if (tries > 300)
        {
            printf("%d intentos de numeros random, rellenando vector...\n", tries);
            break;
        }

        int new_number = get_random_number(1, BINGO_MAX_NUMBER);

        if (number_not_in_vector(number_vector, BINGO_MAX_NUMBER, new_number) != 1)
        {
            --i;
            continue;
        }

        number_vector[i] = new_number;
    }

    if (tries > 300)
    {
        for (int i = 1; i <= BINGO_MAX_NUMBER; ++i)
        {
            int new_number = i;
            if (number_not_in_vector(number_vector, BINGO_MAX_NUMBER, new_number) != 1)
            {
                continue;
            }else{
                for( int j = 0; j < BINGO_MAX_NUMBER; j++ ){
                    if(number_vector[j] == 0){
                        number_vector[j] = new_number;
                    }
                }
            }
        }
    }
}

void reset_random_number_vector(int number_vector[BINGO_MAX_NUMBER])
{
    for (int i = 0; i < BINGO_MAX_NUMBER; ++i)
    {
        number_vector[i] = 0;
    }
}
```
* **number_not_in_vector**: Verifica si el vector no tiene el número especificado.
* **get_random_number_vector**: Trata de llenar el vector con números aleatorios entre 1 y 90 en menos de 300 intentos. Si se pasa, se rellenan los espacios faltantes de una manera más secuencial con los números que falten.
* **reset_random_number_vector**: Reinicia el valor en la posición *i* en el vector, asignándole 0.

```c
void init_players(struct Player *players[NUM_PLAYERS])
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        players[i] = (struct Player *)malloc(sizeof(struct Player));
        players[i]->id = i;
        players[i]->cartones_ganadores = 0;
        players[i]->cartera = 100;
    }
}

void prepare_players(struct Player *players[NUM_PLAYERS])
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        empty_player_cartones(players[i]);

        players[i]->current_number_count = 0;
        
        players[i]->n_cartones = get_random_number_from_1_to_10();

        int deduction = players[i]->n_cartones * 5;
        if( players[i]->cartera - deduction < 0){

            
            if(players[i]->cartera < 5){
                players[i]->n_cartones = 0;
                continue;

            }else{
                players[i]->n_cartones = 1;
                
            }

        }

        fill_player_cartones(players[i]);
    }
}

void empty_player_cartones(struct Player *player)
{
    // llenar cartones
    for (int carton = 0; carton < player->n_cartones; ++carton)
    {
        for (int space = 0; space < BINGO; ++space)
        {
            player->cartones[carton][space][NUMBERS_CARTON] = 0;
        }
    }
}

int get_winner()
{
    return winner;
}
```
* **init_players**: Crea todos los jugadores.
* **prepare_players**: Prepara los jugadores para la partida.
* **empty_player_cartones**: Vacía los cartones de los jugadores.
* **get_winner**: Retorna el id del ganador.

### Procesos del Bingo

#### main.c

```c
void show_count(struct Player *players[NUM_PLAYERS], int recaudado)
{
    printf("\nya no quedan suficientes jugadores, Se Acabo el Bingo.\n");
    printf("1. fichas recaudadas: %d\n  monedas reacudadas en total: %d\n", recaudado, recaudado * 10);

    printf("\n2. Número de cartones ganadores por jugador y en total\n");
    int sum = 0;
    int most_winners = 0;
    int less_winners = 10000;

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        printf("    player %d, cartones ganadores: %d\n", players[i]->id, players[i]->cartones_ganadores);
        sum += players[i]->cartones_ganadores;
        if (players[i]->cartones_ganadores > most_winners)
        {
            most_winners = players[i]->cartones_ganadores;
        }
        if (players[i]->cartones_ganadores < less_winners)
        {
            less_winners = players[i]->cartones_ganadores;
        }
    }
    printf("    total: %d\n", sum);

    int id_most_winners[NUM_PLAYERS];
    int id_less_winners[NUM_PLAYERS];
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (players[i]->cartones_ganadores == most_winners)
        {
            id_most_winners[i] = players[i]->id;
        }
        else
        {
            id_most_winners[i] = 0;
        }
        
        if (players[i]->cartones_ganadores == less_winners)
        {
            id_less_winners[i] = players[i]->id;
        }
        else
        {
            id_less_winners[i] = 0;
        }
    }

    printf("\n3. jugador(es) con mas bingos ganados: \n");
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (id_most_winners[i] != 0)
        {
            printf("    player %d con %d bingos\n", id_most_winners[i], most_winners);
        }
    }

    printf("\n4. jugador(es) con menos bingos ganados:\n");
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (id_less_winners[i] != 0)
        {
            printf("    player %d con %d bingos\n", id_less_winners[i], less_winners);
        }
    }
}
```
* **show_count**: Muestra las cuentas en pantalla cuando cierra el bingo.

```c
void bingo_runner()
{
    int recaudado = 0;
    struct Player *players[NUM_PLAYERS];
    init_players(players);

    int number_vector[BINGO_MAX_NUMBER];

    while (1)
    {
        int round_accumulation = 0;
        reset_random_number_vector(number_vector);
        get_random_number_vector(number_vector);

        printf("number vector created\n");

        prepare_players(players);

        int n_current_players = 0;
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (players[i]->n_cartones > 0)
            {
                n_current_players++;
                int player_deduction = players[i]->n_cartones * 5;
                round_accumulation += player_deduction;
                players[i]->cartera -= player_deduction;
            }
        }

        if (n_current_players < 2)
        {
            show_count(players, recaudado);

            break;
        }

        set_current_players(n_current_players);
        start_locks();
        process_runner(number_vector, players);
        destroy_locks();

        
        int winner = get_winner();
        players[winner - 1]->cartera += round_accumulation * 0.9;
        recaudado += round_accumulation * 0.1;
        printf("ganador de la ultima ronda: player %d\n", winner);
        printf("ganancias del ganador %f\n", round_accumulation * 0.9);
        printf("ganancias del bingo %f\n", round_accumulation * 0.1);

        char c;
        printf("Presione Enter Para la siguiente ronda de Bingo!\n");
        scanf("%c", &c);

    }

    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        free(players[i]);
    }
}
```
* **bingo_runner**: Ejecuta el bingo hasta que queden menos de 2 jugadores.

```c
void process_runner(int number_vector[BINGO_MAX_NUMBER], struct Player *players[NUM_PLAYERS])
{
    int thread_count = NUM_PLAYERS + 1;
    HANDLE threads[thread_count];
    DWORD thread_id;

    printf("thread count %d\n", thread_count);

    printf("creating thread %d\n", 0);
    threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)host_function, (PVOID)number_vector, 0, &thread_id);

    if (threads[0] == NULL)
    {

        ExitProcess(1);

        return;
    }
    printf("created thread %d\n", 0);

    for (int i = 1; i < thread_count; i++)
    {
        DWORD thread_id;
        int player_i = i - 1;

        printf("creating thread %d\n", players[player_i]->id);
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)player_function, (PVOID)players[player_i], 0, &thread_id);
        printf("created thread %d\n", players[player_i]->id);

        if (threads[i] == NULL)

        {
            printf("Error creando thread %d\n", i);
            ExitProcess(1);

            return;
        }
    }

    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
}
```
* **process_runner**: Realiza la ejecución de los hilos del bingo.

```c
int main()
{

    printf("hello world\n");
    time_t t1;
    srand((unsigned)time(&t1));

    bingo_runner();

    return 0;
}
```

* **main**: Activa la semilla y llama a *bingo_runner*



