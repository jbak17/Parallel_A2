//
// Created by jeva on 18/07/17.
//

#include "thread_functions.h"

/*
 * Reader
 */
void * reader( void * );
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
void * wrtr_inc( void * );
	/* lock the data	*/
	/* write to the data	*/
	/* unlock the data	*/

/*
 * Writer (decrementer)
 */
void * wrtr_dec(void *);
	/* lock the data	*/
	/* write to the data	*/
	/* unlock the data	*/

