#include <cstdio>
#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
#include<bits/stdc++.h>
#include <iostream>

using namespace std;

void* reader(void*);
void* writer(void*);

//used to maintain the number of readers currently accessing the resource
int readcount = 0;
//used to maintain the number of writers currently accessing the resource
int writecount = 0;
//shared varibale between processes
int sh_var = 5;

//create read semaphore, write semaphore
sem_t x, y, z, rsem, wsem;

pthread_t r[3], w[2];

void* reader(void* i)
{
    cout << "\n-------------------------";
    cout << "\n\n reader-" << i << " is reading";


    cout << "\nupdated value :" << sh_var;
}

void* writer(void* i)
{
    cout << "\n\n writer-" << i << "is writing";


}


int main()
{
    // Here, readers have higher priority than writer. If a writer wants to write to the resource, 
    // it must wait until there are no readers currently accessing that resource.
    // 
    // Readers-Writers Problem
    // Consider a situation where we have a file shared between many people.

    // If one of the people tries editing the file, no other person should be reading or 
    // writing at the same time, otherwise changes will not be visible to him / her.
    // However if some person is reading the file, then others may read it at the same time.
    // Precisely in OS we call this situation as the readers - writers problem
    //
    // Problem parameters :
    //
    // One set of data is shared among a number of processes
    // Once a writer is ready, it performs its write. Only one writer may write at a time
    // If a process is writing, no other process can read it
    // If at least one reader is reading, no other process can write
    // Readers may not write and only read

    // Expected output:
    // student@sh - 4.4 - desktop:~$ gcc rw1.c - lpthread
    //    student@sh - 4.4 - desktop:~$ . / a.out
    //    ------------------------ -
    //    reader - 0 is reading
    //    updated value : 5

    //    writer - 0 is writing
    //    ------------------------ -
    //    reader - 1 is reading
    //    updated value : 10
    //    ------------------------ -
    //    reader - 2 is reading
    //    updated value : 10
    //    ------------------------ -
    //    reader - 3 is reading
    //    updated value : 10

    //    writer - 3 is writing
    //    ------------------------ -
    //    reader - 4 is reading

    //initialize semaphores for reading
    //and writing to same location. These semaphores
    //will only be shared between threads we will create
    //below.
    sem_init(&x, 0, 1);
    sem_init(&wsem, 0, 1);
    sem_init(&y, 0, 1);
    sem_init(&z, 0, 1);
    sem_init(&rsem, 0, 1);

    //create our reader and writer threads. 
    pthread_create(&r[0], NULL, reader, (void*)0);
    pthread_create(&w[0], NULL, writer, (void*)0);
    pthread_create(&r[1], NULL, reader, (void*)1);
    pthread_create(&r[2], NULL, reader, (void*)2);
    pthread_create(&r[3], NULL, reader, (void*)3);
    pthread_create(&w[1], NULL, writer, (void*)1);
    pthread_create(&r[4], NULL, reader, (void*)4);

    pthread_join(r[0], NULL);
    pthread_join(w[0], NULL);
    pthread_join(r[1], NULL);
    pthread_join(r[2], NULL);
    pthread_join(r[3], NULL);
    pthread_join(w[1], NULL);
    pthread_join(r[4], NULL);




    //Producer-Consumer Problem
    //Problem Statement
    //    The producer - consumer problem is a set of problems with lots of variants.In this tutorial, we focus on the simplest version :
    //    The system has one producer threadand one consumer thread.
    //    The size of the buffer between producerand consumer is exactly one.In other words, the producer will wait for 
    //    the consumer to consume the product before producing the next one.
    //    The critical point is the producer and the consumer both need to access the shared resource(buffer) without knowing what the other is doing.
    //    We must handle the synchronization properly so that no data provided by the producer is lost
    //    (producer generates the next product before consumer accept) or duplicated(producer fails to update before consumer consume obtain next one).
    //    To achieve the goal, the desired solution should guarantee :
    //    The access of data by two threads are mutually exclusive. When one is accessing the data, the other must be blocked.
    //    The activity of producerand consumer are alternating in a “ping - pong” pattern. In an efficient implementation, the consumer launches
    //    immediately after the producer completes, and vice versa



    printf("hello from %s!\n", "Linux_Semaphore_and_mutex_app");
    return 0;
}