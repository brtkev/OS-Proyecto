#include <stdio.h>
#include "lock.h"
#include <pthread.h>
#include <windows.h>

#define THREADCOUNT 30

DWORD WINAPI reader_function(DWORD i)
{
    printf("thread number %d running...\n", i);
    reader_lock();
    printf("CRITIAL SECTION - Reader Function\n");
    reader_unlock();
    printf("thread number %d finishing...\n", i);
}

DWORD WINAPI writer_function(DWORD i)
{
    printf("thread number %d running...\n", i);
    writer_lock();
    printf("CRITIAL SECTION - writer Function\n");
    writer_unlock();
    printf("thread number %d finishing...\n", i);
}

DWORD WINAPI admin_function(DWORD i)
{
    printf("thread number %d running...\n", i);
    admin_lock();
    printf("CRITIAL SECTION - admin Function\n");
    admin_unlock();
    printf("thread number %d finishing...\n", i);
}

void process_runner(){
    HANDLE threads[THREADCOUNT];
    DWORD i;

    for(i=0;i<THREADCOUNT;i++)
    {
        DWORD thread_id;
        if(i % 3 == 0){
            threads[i] = CreateThread(NULL ,0 , (LPTHREAD_START_ROUTINE)admin_function , (PVOID)i ,0 ,&thread_id );

        }else if( i % 2 == 0){
            threads[i] = CreateThread(NULL ,0 , (LPTHREAD_START_ROUTINE)writer_function , (PVOID)i ,0 ,&thread_id );

        }else{
            threads[i] = CreateThread(NULL ,0 , (LPTHREAD_START_ROUTINE)reader_function , (PVOID)i ,0 ,&thread_id );

        }
            

        if(threads[i]== NULL)

        {
            printf("LCreateThread() failed, error %u\n", GetLastError());

            ExitProcess(1);

            return;

        }
        // else printf("LCreateThread() is OK, thread ID %u\n", thread_id);
    }
    WaitForMultipleObjects(THREADCOUNT,threads,TRUE, INFINITE);
}

int main()
{

    
    // Initialize the semaphores.
    start_locks();
    
    process_runner();

    // Destroy the semaphores
    destroy_locks();

    return 0;
}
