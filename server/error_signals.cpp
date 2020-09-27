#include "error_signals.h"
#include <stdio.h>

error_signals *error_thread_list;

pid_t error_signals::GetThreadID()
{
	pid_t tid = syscall(SYS_gettid);
	return tid;
}

error_signals *error_signals::GetThread(pid_t thread_id)
{
	error_signals *loop = error_thread_list;
	while (loop != nullptr)
	{
		if ((loop->in_use) && (loop->thread_id == thread_id))
		{
			break;
		}
		loop = loop->next;
	}	
	return loop;
}

void error_signals::DeleteThreads()
{
	error_signals *item;
	while (error_thread_list != nullptr)
	{
		item = error_thread_list;		
		error_thread_list = item->next;
		delete item;
	}
	return;
}

error_signals *error_signals::AddThread(pid_t thread_id)
{
	error_signals *loop = error_thread_list;
	while (loop != nullptr)
	{
		if ((!loop->in_use) || (loop->thread_id == thread_id))
		{
			break;
		}
		loop = loop->next;
	}
	if (loop == nullptr)
	{
		loop = new error_signals;
		if (loop != nullptr)
		{
			loop->in_use = true;
			loop->thread_id = thread_id;
			loop->next = error_thread_list;
			error_thread_list = loop;
		}
	}
	return loop;
}

void error_signals::ReleaseThread(pid_t thread_id)
{
	error_signals *loop = error_thread_list;
	while (loop != nullptr)
	{
		if (loop->thread_id == thread_id)
		{
			loop->in_use = false;
			break;
		}
		loop = loop->next;
	}
	return;
}

void error_signals::Handler(int signum)
{
	pid_t tid = error_signals::GetThreadID();
	error_signals *error_thread = error_signals::GetThread(tid);
	if (error_thread != nullptr)
	{
		error_thread->SaveLineNumbers();
		longjmp(error_thread->position,signum);
	}
	return;	
}

void error_signals::SaveLineNumbers()
{
	// Save line numbers before the stack is trashed by longjmp.
	int loop;
	int max;
	int *lineNumber;
	
	max = LineNumberStack;
	if (max >0)
	{
		if (max > CALL_STACK_SIZE+1)
		{
			max = CALL_STACK_SIZE+1;
		}
		for(loop=0;loop<max;loop++)
		{
			lineNumber = LineNumbers[loop];
			if (lineNumber == nullptr) {
				SavedLineNumbers[loop] = 0;
			} else {
				SavedLineNumbers[loop] = *lineNumber;				
			}
		}
	}
	return;
}


void error_signals::AddHandlers()
{
	/*struct sigaction sa;
	sa.sa_handler = error_signals::Handler;	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;// Restart functions if interrupted by handler
	if (sigaction(SIGFPE, &sa, NULL) == -1)
	{
		break;
		// Handle error
		//printf("Error calling signaction.\n");
	}*/	
	signal (SIGFPE, error_signals::Handler); // division by 0
	signal (SIGILL, error_signals::Handler); // illegal instruction
	signal (SIGSEGV, error_signals::Handler); // bad memory read/write
	signal (SIGBUS, error_signals::Handler); // access misalligned memory or non-existent memory	
	return;
}

void error_signals::RemoveHandlers()
{
	// Make sure the signals don't point to some function that might not exist in the future. (dll unloads).
	signal (SIGFPE, SIG_DFL); // division by 0
	signal (SIGILL, SIG_DFL); // illegal instruction
	signal (SIGSEGV, SIG_DFL); // bad memory read/write
	signal (SIGBUS, SIG_DFL); // access misalligned memory or non-existent memory	
	return;
}

StackTrace *error_signals:: GetStackTrace()
{
	SaveLineNumbers();
	return this;
}

//webpage_list[] = { "foo", "bar",0 };
