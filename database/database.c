#include <stdio.h>
#include "read.h"
#include <pthread.h>
#include <windows.h>

// Create two semaphores: one for readers and one for writers.
HANDLE reader_semaphore;
HANDLE writer_semaphore;
int num_readers = 0;

// Reader lock
void reader_lock()
{
    printf("reader Lock\n");
    // Wait for the reader semaphore to be signaled.
    WaitForSingleObject(reader_semaphore, INFINITE);
    // If the first reader, wait for the writer semaphore to be signaled.
    if (num_readers == 0)
    {
        WaitForSingleObject(writer_semaphore, INFINITE);
    }

    // Increment the number of readers.
    num_readers++;
    printf("reader_lock - current number of readers: %d\n", num_readers);

}

// Reader unlock
void reader_unlock()
{
    printf("reader unlock\n");
    // Decrement the number of readers.
    num_readers--;
    // Release the reader semaphore.
    ReleaseSemaphore(reader_semaphore, 1, NULL);

    // If the last reader, signal the writer semaphore.
    if (num_readers == 0)
    {
        ReleaseSemaphore(writer_semaphore, 1, NULL);
    }
}

// Writer lock
void writer_lock()
{
    printf("write Lock\n");
    // Wait for the writer semaphore to be signaled.
    WaitForSingleObject(writer_semaphore, INFINITE);

    printf("writer_lock - current number of readers: %d\n", num_readers);
    // Wait for all readers to finish reading.
    while (num_readers > 0)
    {
        // Wait for the reader semaphore to be signaled.
        WaitForSingleObject(reader_semaphore, INFINITE);

        // // Decrement the number of readers.
        // num_readers--;

        // Release the reader semaphore.
        ReleaseSemaphore(reader_semaphore, 1, NULL);
    }
}

// Writer unlock
void writer_unlock()
{
    printf("write unlock\n");
    // Signal the writer semaphore.
    ReleaseSemaphore(writer_semaphore, 1, NULL);
}

DWORD WINAPI reader_function()
{
    reader_lock();
    printf("Reader Function\n");
    reader_unlock();
}

DWORD WINAPI writer_function()
{
    writer_lock();
    printf("Writer Function\n");
    writer_unlock();
}

int main()
{
    // Initialize the semaphores.
    printf("Init sem\n");
    reader_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    writer_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // Create two threads: one reader and one writer.
    HANDLE reader_thread = CreateThread(NULL, 0, reader_function, NULL, 0, NULL);
    HANDLE writer_thread = CreateThread(NULL, 0, writer_function, NULL, 0, NULL);

    // Wait for both threads to finish.
    WaitForSingleObject(reader_thread, INFINITE);
    WaitForSingleObject(writer_thread, INFINITE);

    // Destroy the semaphores
    CloseHandle(reader_semaphore);
    CloseHandle(writer_semaphore);

    return 0;
}
