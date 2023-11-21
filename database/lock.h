#pragma once

void start_locks();
void destroy_locks();

void reader_lock();
void reader_unlock();

void writer_lock();
void writer_unlock();

void admin_lock();
void admin_unlock();