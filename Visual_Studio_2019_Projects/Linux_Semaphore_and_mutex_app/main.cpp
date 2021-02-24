#include <cstdio>


int main()
{
    //Readers-Writers Problem
    //Consider a situation where we have a file shared between many people.

    //If one of the people tries editing the file, no other person should be reading or writing at the same time, otherwise changes will not be visible to him / her.
    //However if some person is reading the file, then others may read it at the same time.
    //Precisely in OS we call this situation as the readers - writers problem
    //
    //Problem parameters :
    //
    //One set of data is shared among a number of processes
    //Once a writer is ready, it performs its write.Only one writer may write at a time
    //If a process is writing, no other process can read it
    //If at least one reader is reading, no other process can write
    //Readers may not write and only read

    //Producer-Consumer Problem
    //Problem Statement
    //    The producer - consumer problem is a set of problems with lots of variants.In this tutorial, we focus on the simplest version :
    //The system has one producer threadand one consumer thread.
    //    The size of the buffer between producerand consumer is exactly one.In other words, the producer will wait for 
    //    the consumer to consume the product before producing the next one.
    //    The critical point is the producerand the consumer both need to access the shared resource(buffer) without knowing what the other is doing.
    //    We must handle the synchronization properly so that no data provided by the producer is lost
    //    (producer generates the next product before consumer accept) or duplicated(producer fails to update before consumer consume obtain next one).
    //    To achieve the goal, the desired solution should guarantee :
    //The access of data by two threads are mutually exclusive.When one is accessing the data, the other must be blocked.
    //    The activity of producerand consumer are alternating in a “ping - pong” pattern.In an efficient implementation, the consumer launches
    //    immediately after the producer completes, and vice versa



    printf("hello from %s!\n", "Linux_Semaphore_and_mutex_app");
    return 0;
}