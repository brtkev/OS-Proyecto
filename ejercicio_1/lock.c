#include <windows.h>
#include <stdio.h>
#include "logger.h"

// Create two semaphores: one for admins and one for writers.
HANDLE writer_semaphore;
HANDLE admin_semaphore;
int num_readers = 0;
int num_admins = 0;

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

// Reader lock
void reader_lock()
{
    // Increment the number of readers.
    num_readers++;
    console_log(1, "READER - new reader, current number of readers: %d\n",   num_readers);

    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(writer_semaphore, INFINITE);

    //wait for admin
    WaitForSingleObject(admin_semaphore, INFINITE);

    console_log(1,"READER - CRITICAL SECTION\n");
}

// Reader unlock
void reader_unlock()
{
    num_readers--;
    console_log(1, "READER - CRITICAL SECTION FIN, number of reader %d\n", num_readers);
    ReleaseSemaphore(admin_semaphore, 1, NULL);
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}

// Writer lock
void writer_lock()
{
    console_log(1, "WRITER - new writer\n");
    // printf("WRITER - new writer\n");
    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(writer_semaphore, INFINITE);
    WaitForSingleObject(admin_semaphore, INFINITE);
    console_log(1, "WRITER - Critic section\n");
    // printf("WRITER - Critic section\n");
}

// Writer unlock
void writer_unlock()
{
    console_log(1, "WRITER - Critic section FIN\n");
    // printf("WRITER - Critic section FIN\n");
    // Signal the writer semaphore.
    ReleaseSemaphore(admin_semaphore, 1, NULL);
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}

void admin_lock(){
    //wait for admin
    ++num_admins;
    console_log(1, "ADMIN - new admin, number of admins %d\n", num_readers);
    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(admin_semaphore, INFINITE);

    console_log(1, "ADMIN - CRITICAL SECTION\n");
    // printf("ADMIN - CRITICAL SECTION\n");

}

void admin_unlock(){
    //release admin
    --num_admins;
    console_log(1 ,"ADMIN - CRITICAL SECTION FIN, number of admins %d\n", num_admins);
    // Signal the writer semaphore.
    ReleaseSemaphore(admin_semaphore, 1, NULL);
}