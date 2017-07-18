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


typedef struct shared_data {
	int sum;
	int last_inc;
	int last_dec;
	int writes;
} s_data;

int main(int argc, char * argv[]){
/*
 * Shared data
 */
	s_data *shared;
	s_data.sum = 0;
	s_data.writes = 0;

/*
 * Create locks
 */

/*
 * Create threads
 */


/*
 * Wait for threads to complete
 */
	return 1;
}