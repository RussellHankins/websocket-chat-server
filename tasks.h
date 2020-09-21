#ifndef __TASKS_H
#define __TASKS_H
#include "task.h"
#include <pthread.h>
#include "concurrent_queue.h"

// This class is a queue of tasks.
// add_task and do_tasks can be called from separate threads.
// add_task will be called from the main thread.
// do_tasks will be called from the background thread.
// All do_tasks does is process each task in chatroom_tasks one by one by calling the do_task() method of the task object.
// wait_for_tasks will be called from the background thread.
class tasks
{
	public:
	tasks();
	~tasks();
	void add_task(task *task); // Adds a task to the list of tasks to do. Calling add_task might signal other threads that a task is ready to process.
	void do_tasks(); // Does all tasks. Returns when no tasks are waiting.
	void wait_for_tasks(int delay_in_seconds); // Wait until there are tasks to do or delay_in_seconds, whichever comes first. If delay_in_seconds = 0, then wait forever.
	private:
	concurrent_queue<task *>*chatroom_tasks; // List of tasks.
	pthread_mutex_t queue_mutex; // This mutex should be locked if no tasks are waiting.
};
#endif
