//
// Created by jeva on 18/07/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread_functions.h"

/*
 * Reader
 */
void *reader( void *arg ){
	pthread_mutex_lock(&data_lock);
	printf("Reader created\n");
	pthread_exit(NULL);
	}
//lock count_lock

//readers_count++

//if( readers_count == 1 )
//lock data_lock

//unlock count_lock
//read the data
//		lock count_lock

//		readers_count--

//if( readers_count == 0 )
//unlock data_lock

//unlock count_lock

/*
 * Writer (incrementer)
 */
void *wrtr_inc( void *arg ){
	printf("Incrementer created\n");
	pthread_exit(NULL);
	}
	/* lock the data	*/
	/* write to the data	*/
	/* unlock the data	*/

/*
 * Writer (decrementer)
 */
void *wrtr_dec( void *arg ){
	printf("Decrementer created\n");
	pthread_exit(NULL);
	}
	/* lock the data	*/
	/* write to the data	*/
	/* unlock the data	*/

