#include <windows.h>
#include <stdio.h>

// Create two semaphores: one for readers and one for writers.
HANDLE reader_semaphore;
HANDLE writer_semaphore;
HANDLE admin_semaphore;
int num_readers = 0;

void start_locks(){
    // Initialize the semaphores.
    printf("SET - Initialize the semaphores\n");
    reader_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    writer_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    admin_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

void destroy_locks(){
    // Destroy the semaphores
    printf("SET - destroy the semaphores\n");
    CloseHandle(reader_semaphore);
    CloseHandle(writer_semaphore);
    CloseHandle(admin_semaphore);
}

// Reader lock
void reader_lock()
{
    // Increment the number of readers.
    num_readers++;
    printf("READER - new reader, current number of readers: %d\n", num_readers);

    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(writer_semaphore, INFINITE);

    printf("READER - CRITICAL SECTION\n");
}

// Reader unlock
void reader_unlock()
{
    num_readers--;
    printf("READER - CRITICAL SECTION FIN\n");
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}

// Writer lock
void writer_lock()
{
    printf("WRITER - new writer\n");
    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(writer_semaphore, INFINITE);

    printf("WRITER - Critic section\n");
}

// Writer unlock
void writer_unlock()
{
    printf("WRITER - Critic section FIN\n");
    // Signal the writer semaphore.
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}

void admin_lock(){
    //wait for admin
    printf("ADMIN - new admin\n");
    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(admin_semaphore, INFINITE);

    printf("ADMIN - CRITICAL SECTION\n");

}

void admin_unlock(){
    //release admin
    printf("ADMIN - CRITICAL SECTION FIN\n");
    // Signal the writer semaphore.
    ReleaseSemaphore(admin_semaphore, 1, NULL);
}