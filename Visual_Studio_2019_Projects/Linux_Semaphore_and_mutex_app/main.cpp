// Reader-Writer problem using monitor
#include <iostream> 
#include <pthread.h> 
#include <unistd.h> 
#include <fstream>
using namespace std;

class monitor {
	//The monitor is one of the ways to achieve Process synchronization.
	//The monitor is supported by programming languages to achieve mutual exclusion between processes.
	//It is the collection of condition variables and procedures combined together in a special kind of module or a package.
	//The processes running outside the monitor can’t access the internal variable of the monitor but can call procedures of the monitor.
	//Only one process at a time can execute code inside monitors.
private:
	// no. of readers 
	int rcnt;

	// no. of writers 
	int wcnt;

	// no. of readers waiting 
	int waitr;

	// no. of writers waiting 
	int waitw;

	// condition variable to check whether reader can read 
	pthread_cond_t canread;

	// condition variable to check whether writer can write 
	pthread_cond_t canwrite;

	// mutex for synchronisation 
	pthread_mutex_t condlock;

public:
	monitor()
	{
		rcnt = 0;
		wcnt = 0;
		waitr = 0;
		waitw = 0;

		//initialize condition variables to defualt attributes
		pthread_cond_init(&canread, NULL);
		pthread_cond_init(&canwrite, NULL);
		//initialize mutex to default attributes
		pthread_mutex_init(&condlock, NULL);
	}

	// mutex provide synchronisation so that no other thread 
	// can change the value of data 
	void beginread(int i)
	{
		//lock mutex so that i can read
		pthread_mutex_lock(&condlock);

		// if there are active or waiting writers 
		if (wcnt == 1 || waitw > 0) {
			// then we have to wait so
			// incrementing waiting readers 
			waitr++;

			//block because we are waiting to read
			// reader suspended 
			pthread_cond_wait(&canread, &condlock);
			//decrement waiting readers 
			//because it's our turn to read
			waitr--;
		}

		// there are no active or waiting writers
		// So we can read the resource 
		rcnt++;
		//unlock the mutex since we are done reading
		pthread_mutex_unlock(&condlock);
		//let other readers know they can read
		pthread_cond_broadcast(&canread);

		cout << "reader " << i << " is reading\n";
		std::ifstream is("example.txt", std::ifstream::binary);
		if (is) {
			// get length of file:
			is.seekg(0, is.end);
			int length = is.tellg();
			is.seekg(0, is.beg);

			char* buffer = new char[length];

			std::cout << "Reading " << length << " characters... \n";
			// read data as a block:
			is.read(buffer, length);

			if (is)
				std::cout << "all characters read successfully. \n";
			else
				std::cout << "error: only " << is.gcount() << " could be read\n";
			is.close();

			// ...buffer contains the entire file...

			delete[] buffer;
		}

	}

	void endread(int i)
	{
		// if there are no readers left then writer enters monitor 
		pthread_mutex_lock(&condlock);

		//pre-decrement readers count
		//to signal that writers can
		//write if all readers are done reading.
		if (--rcnt == 0)
			pthread_cond_signal(&canwrite);

		//all done reading now so release the lock
		pthread_mutex_unlock(&condlock);
		cout << "reader " << i << " is done reading\n\n";
	}

	void beginwrite(int i)
	{
		//It's time to write so acquire mutex lock for writing
		pthread_mutex_lock(&condlock);

		// a writer can enter when there are no active 
		// or waiting readers or other writer 
		if (wcnt == 1 || rcnt > 0) {
			//increase number of waiting writers
			++waitw;
			//there are writers waiting or reader waiting so
			//block until its our turn to write
			pthread_cond_wait(&canwrite, &condlock);
			--waitw;
		}
		wcnt = 1;
		cout << "writer " << i << " is writing\n";
		ofstream myfile;
		cout << "Writing this to a file." <<i<<endl;
		myfile.open("example.txt");
		myfile << "Writing this to a file.\n" + i;
		myfile.close();
		pthread_mutex_unlock(&condlock);
	}

	void endwrite(int i)
	{
		pthread_mutex_lock(&condlock);
		wcnt = 0;

		// if any readers are waiting, threads are unblocked 
		if (waitr > 0)
			//signal to readers that they can read
			pthread_cond_signal(&canread);
		else //signal to next writer so they can write
			pthread_cond_signal(&canwrite);
		pthread_mutex_unlock(&condlock);
		cout << "writer " << i << " is done writing\n\n";
	}

}// global object of monitor class 
M;

void* reader(void* id)
{
	int c = 0;
	int i = *(int*)id;

	// each reader attempts to read 5 times 
	while (c < 5) {
		usleep(0.75);
		M.beginread(i);
		M.endread(i);
		c++;
	}
}

void* writer(void* id)
{
	int c = 0;
	int i = *(int*)id;

	// each writer attempts to write 5 times 
	while (c < 5) {
		usleep(0.75);
		M.beginwrite(i);
		M.endwrite(i);
		c++;
	}
}
const int THREADS = 5;
int main()
{
	ofstream myfile;
	myfile.open("example.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
	//create 5 reader threads and 5 writer threads
	pthread_t r[THREADS], w[THREADS];

	//create an ID array that contains ID's of each thread
	int id[5];
	//launch reader and writer threads with specified id
	for (int i = 0; i < THREADS; i++) {
		id[i] = i;
		// creating threads which execute reader & writer functions
		pthread_create(&r[i], NULL, &reader, &id[i]);
		pthread_create(&w[i], NULL, &writer, &id[i]);
	}

	//ensure that this thread does not exit before the reader
	//and writer threads terminate.
	for (int i = 0; i < THREADS; i++) {
		pthread_join(r[i], NULL);
		pthread_join(w[i], NULL);
	}
	return 0;
}
