# OS-Proyecto
Proyecto Para Sistemas Operativos 2023



# Ejercicio 1

## 1. Make File
Para compilar y ejecutar el ejercicio 1, debemos ejecutar en el terminal los siguientes pasos:


```shell
    make 1
    ./1
```

### explicacion

dentro del archivo [Makefile](./Makefile) encontraremos 2 funciones

```make
1: 1.exe
	echo "make 1.exe"

1.exe: $(EJ_1_FILES)
	gcc $(CFLAG) -o $@ $(EJ_1_FILES)
```

la funcion "1", llama a la funcion "1.exe" y luego ejecuta el echo

la funcion "1.exe", llama a EJ_1_FILES, los cuales son todos los archivos .c del directorio [ejercicio 1](./ejercicio_1), con estos objetos dentro de la funcion, luego se compilan usando el comando gcc con la bandera -o y se usa el nombre de la funcion (1.exe) utilizando $@

## 2. Ejecucion

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

### implementacion de los semaforos
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

###