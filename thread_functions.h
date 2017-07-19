//
// Created by jeva on 18/07/17.
//

#ifndef PARALLEL_A2_THREAD_FUNCTIONS_H
#define PARALLEL_A2_THREAD_FUNCTIONS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

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

/*
 * Reader
 */
void * reader( void * );
/*
 * Writer (incrementer)
 */
void * wrtr_inc( void * );
/*
 * Writer (decrementer)
 */
void * wrtr_dec( void * );

void writer_helper(int, s_data);


#endif //PARALLEL_A2_THREAD_FUNCTIONS_H

