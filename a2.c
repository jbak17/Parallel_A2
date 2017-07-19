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
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "thread_functions.h"

#define TH_NO 15



typedef struct shared_data {
	int sum;
	int reader_count;
	int last_inc;
	int last_dec;
	int writes;

	pthread_mutex_t sum_lock; //lock sum
	pthread_mutex_t read_lock; //lock for reader count
} s_data;

void * reader( void * );
void * wrtr_inc( void * );
void * wrtr_dec( void * );

int main(int argc, char * argv[]){

	int i;
	/*
	 * Shared data
	 */
	s_data shared;

	shared.sum = 0;
	shared.writes = 0;
	shared.last_dec = 0;
	shared.last_inc = 0;
	shared.reader_count = 0;

	/*
	 * Create locks
	 */
	pthread_mutex_init (&shared.sum_lock, NULL);
	pthread_mutex_init (&shared.read_lock, NULL);
	/*
	 * Create threads
	 */
	pthread_t thread[TH_NO];

	void * (*pmain[3])(void *) = {reader, wrtr_dec, wrtr_inc};
	for(i = 0; i < TH_NO; i++){
		int thread_type = rand() %3;
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
		   shared.last_inc, shared.last_dec,
		   shared.writes, shared.sum);

	return 1;
}

/*
 * Reader
 */
void *reader( void *arg ){

	s_data input = *((s_data *) arg);

	//lock reader count
	pthread_mutex_lock (&input.read_lock);

	input.reader_count++;

	if( input.reader_count == 1 )
		//lock data for reading
		pthread_mutex_lock (&input.sum_lock);
		//unlock reader counter
		pthread_mutex_unlock (&input.read_lock);
		//read the locked data
		fprintf(stderr, "Reader %d got %d\n", (int)pthread_self(), input.sum );
		//	lock the reader counter
		pthread_mutex_lock(&input.read_lock);
		input.reader_count--;

	if( input.reader_count == 0 )
		//unlock data_lock
		pthread_mutex_unlock (&input.read_lock);

	//unlock reader counter
	pthread_mutex_unlock (&input.read_lock);

	pthread_exit (NULL);
}


/*
 * Writer (decrementer)
 */
void *wrtr_dec( void *arg ){

	s_data input = *((s_data *) arg);

	/* lock the data	*/
	pthread_mutex_lock (&input.sum_lock);
	/* write to the data	*/
	input.sum --;
	input.writes ++;
	input.last_dec = (int)pthread_self();
	fprintf(stderr, "Decrementer %d set sum = %d\n", input.last_dec, input.sum);
	/* unlock the data	*/
	pthread_mutex_unlock (&input.sum_lock);
	pthread_exit (NULL);
}

/*
 * Writer (incrementer)
 */
void *wrtr_inc( void *arg ){

	s_data input = *((s_data *) arg);

	/* lock the data	*/
	pthread_mutex_lock (&input.sum_lock);
	/* write to the data	*/
	input.sum ++;
	input.writes ++;
	input.last_inc = pthread_self();
	fprintf(stderr, "Incrementer %d set sum = %d\n", input.last_dec, input.sum);
	/* unlock the data	*/
	pthread_mutex_unlock (&input.sum_lock);
	pthread_exit (NULL);
}




