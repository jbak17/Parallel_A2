# Parallel_A2
	Variation on readers and writers problem where we can have many concurrent readers but only one writer.

# pseudo code of algorithms
	shared semaphore count_lock
	shared int readers_count initially 0
	shared semaphore data_lock
	shared data

# reader algorithm:
	lock count_lock

	readers_count++

	if( readers_count == 1 )
	  lock data_lock

	unlock count_lock
	read the data
	lock count_lock

	readers_count--

	if( readers_count == 0 )
	  unlock data_lock

	unlock count_lock

# writers algorithm
	lock data_lock
	write to the data
	unlock data_lock

# Assignment particulars:
	No command line arguments.

Three kinds of threads: 
  readers
  incrementers (writers)
  decrementers (writers)
  
Using pthreads, create a random number of each kind of thread. 

The shared data should be in a stuct with fields for the sum, last incrementer id, last decrementer id, and number of write so far. 

# Output
All threads should print their identity and their operation on the sum each time they access the shared data.
