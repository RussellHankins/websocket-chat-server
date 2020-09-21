#include "tasks.h"
#include <time.h>

// Here's some information on pthread_mutex:
// https://www.ibm.com/support/knowledgecenter/ssw_aix_72/p_bostechref/pthread_mutex_timedlock.html

tasks::tasks()
{
	chatroom_tasks = new concurrent_queue<task *>();	
	queue_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&queue_mutex,NULL);
	pthread_mutex_lock(&queue_mutex);
}

tasks::~tasks()
{
	delete chatroom_tasks;
	chatroom_tasks = nullptr;
	pthread_mutex_destroy(&queue_mutex);	
}

void tasks::wait_for_tasks(int delay_in_seconds)
{
	// This function waits for a task.
	// Just because this function returns, doesn't mean there's a task.
	// This was on purpose to prevent a race condition.
	// If there's a task waiting, this function returns immediately.
	// If no task is waiting, this function returns in 1 second.
	struct timespec abs_timeout;
	if (!chatroom_tasks->empty())
	{
		return;
	}	
	if (delay_in_seconds <=0) {
		pthread_mutex_lock(&queue_mutex);		
	} else {
		timespec_get(&abs_timeout,TIME_UTC);
		abs_timeout.tv_sec += delay_in_seconds;
		pthread_mutex_timedlock(&queue_mutex,&abs_timeout);
	}
	if (!chatroom_tasks->empty()) {		
		// If there are tasks, then the mutex should be unlocked.
		pthread_mutex_trylock(&queue_mutex); // Make sure it's locked so unlock doesn't fail if it's already unlocked.
		pthread_mutex_unlock(&queue_mutex);
	}
	return;	
}

void tasks::add_task(task *task)
{
	bool was_queue_empty;
	was_queue_empty = chatroom_tasks->empty();
	chatroom_tasks->enqueue(task);
	if (was_queue_empty) {
		// Unlock the mutex to show that something is now on the queue.
		pthread_mutex_trylock(&queue_mutex); // Make sure it's locked so unlock doesn't fail if it's already unlocked.
		pthread_mutex_unlock(&queue_mutex);	
	}
	return;
}

void tasks::do_tasks()
{
	// sometimes tasks create other tasks.
	// If run_async is false, this routine is called to make sure every task runs.	
	task *next_task;
	while (!chatroom_tasks->empty())
	{
		chatroom_tasks->dequeue(next_task);
		next_task->dotask();		
		delete next_task;
	}
	// The queue should be empty. Try to lock it.
	pthread_mutex_trylock(&queue_mutex);	
	return;
}
