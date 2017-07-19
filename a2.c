/*
 * Implementation of the readers and writers problem in C
 * using pthreads with two kinds of writers: incrementers and
 * decrementers.
 *
 * A random number of incrementers and decrementers is created,
 * along with a random number of readers.
 *
 * At each stage the threads print out what they have done.
 * Once all the threads have completed the main function
 * returns the result of the threads.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#include "thread_functions.h"

#define TH_NO 50

//Storage for number of each type of thread
typedef struct thread_types{
	int decrementer, incrementer, reader;
} Types;

/*
 * Function to update the storage of the number
 * of threads of each kind. Accepts an initialised
 * struct of Types, matches 'thread' against reader/
 * writer, updates record and returns new id.
 */
int thread_id(Types *arg, int thread){

	switch (thread){
		case 0:
			arg->reader += 1;
			return arg->reader;
		case 1 :
			arg->decrementer +=1;
			return arg->decrementer;
		case 2:
			arg->incrementer += 1;
			return arg->incrementer;
	}
	perror("Problem in thread_id switch");
	exit(EXIT_FAILURE);
}

typedef struct shared_data {
	int *sum;
	int *reader_count;
	int *last_inc;
	int *last_dec;
	int *writes;

	int thread_id;

	pthread_mutex_t data_lock; //lock sum
	pthread_mutex_t read_lock; //lock sum

} s_data;

/*	Thread types */
void * reader( void * );
void * wrtr_inc( void * );
void * wrtr_dec( void * );

/*
 *
 * Provides for the creation of customised writers
 * that amend the shared sum by the value of the
 * 'add' variable.
 *
 */
void writer_helper(int add, s_data input){

	/* lock the data	*/
	pthread_mutex_lock (&input.data_lock);

	/* write to the data	*/
	(*input.sum) += add;
	(*input.writes) ++;
	if(add > 0) {
		*input.last_inc = input.thread_id;
		fprintf(stderr, "Incrementer %d set sum = %d\n", *input.last_dec, *input.sum);
	} else {
		*input.last_dec = input.thread_id;
		fprintf(stderr, "Decrementer %d set sum = %d\n", *input.last_dec, *input.sum);
	}

	/* unlock the data	*/
	pthread_mutex_unlock (&input.data_lock);

	return;
}

int main(int argc, char * argv[]){

	int i;
	int total, rcount, lst_inc, lst_dec, no_writes;
	total = rcount = no_writes = 0;

	/*
	 * Shared data
	 */
	s_data shared;

	shared.sum = &total;
	shared.reader_count = &rcount;
	shared.writes = &no_writes;
	shared.last_inc = &lst_inc;
	shared.last_dec = &lst_dec;

	/*
	 * Create locks
	 */
	pthread_mutex_init (&shared.data_lock, NULL);
	pthread_mutex_init (&shared.read_lock, NULL);

	/*
	 * Create threads
	 */
	pthread_t thread[TH_NO];
	//keep track of how many threads of each kind.
	Types threads;
	threads.incrementer = threads.decrementer = threads.reader = 0;

	void * (*pmain[3])(void *) = {reader, wrtr_dec, wrtr_inc};
	for(i = 0; i < TH_NO; i++){
		int thread_type = rand() %3;
		shared.thread_id = thread_id(&threads, thread_type);
		if(pthread_create(&thread[i],NULL,pmain[thread_type],&shared) != 0){
			fprintf(stderr, "Thread creation failed in client\n");
			exit(EXIT_FAILURE);
		}
	}

	/*
	 * Wait for threads to complete
	 */
	for(i = 0; i < TH_NO; i++)
		pthread_join(thread[i],NULL);
/*
 * Output results
 */
	printf("The final state of the data is:\n"
				   "\tLast incrementer: %d\n"
				   "\tLast decrementer: %d\n"
				   "\tTotal writers: %d\n"
				   "\tSum: %d\n",
		   lst_inc, lst_dec,
		   no_writes, total);

	return 1;
}

/*
 * Reader
 */
void *reader( void *arg ){

	s_data input = *((s_data *) arg);

	//lock reader count
	pthread_mutex_lock (&input.read_lock);

	(*input.reader_count) ++;

	if( *input.reader_count == 1 ){
		//lock data for reading
		pthread_mutex_lock (&input.data_lock);
	}

	//unlock reader counter
	//pthread_mutex_unlock (&input.read_lock);

	//read the locked data
	fprintf(stderr, "Reader %d got %d\n", input.thread_id, *input.sum );
	//	lock the reader counter
	//pthread_mutex_lock(&input.read_lock);
	(*input.reader_count)--;

	if( *input.reader_count == 0 )
		//unlock data_lock
		pthread_mutex_unlock (&input.data_lock);

	//unlock reader counter
	pthread_mutex_unlock (&input.read_lock);

	pthread_exit (NULL);
}


/*
 * Writer (decrementer)
 */
void *wrtr_dec( void *arg ){
	s_data input = *((s_data *) arg);

	writer_helper(-1, input);
	pthread_exit (NULL);
}

/*
 * Writer (incrementer)
 */
void *wrtr_inc( void *arg ){

	s_data input = *((s_data *) arg);

	writer_helper(1, input);
	pthread_exit (NULL);
}




