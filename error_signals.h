#ifndef __ERROR_SIGNAL_H
#define __ERROR_SIGNAL_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#include <setjmp.h>  // Yes, I used setjmp and longjmp. What cha gonna to do about it?
#include "StackTrace.h"
#include <sys/types.h>

class error_signals:public StackTrace
{
	public:
	pid_t thread_id;
	bool in_use;
	jmp_buf position;
	error_signals *next;
	int *LineNumbers[CALL_STACK_SIZE]; // These pointers point to the LineNumber property of the Debug object for that stack layer.	
	void SaveLineNumbers(); // Save line numbers before the stack is trashed by longjmp.
	static error_signals *GetThread(pid_t thread_id); // Returns a thread from error_thread_list matching thread_id or nullptr.
	static error_signals *AddThread(pid_t thread_id); // Adds a thread to error_thread_list.	
	static void ReleaseThread(pid_t thread_id); // Set an error signal in error_thread_list in_use = false for the thread_id.
	static pid_t GetThreadID(); // Return the thread id of the current thread.
	static void Handler(int signum); // The signal handler function.
	static void AddHandlers(); // Make the signal handlers point to error_signals::Handler.
	static void DeleteThreads(); // Delete all items in the error_thread_list.
	static void RemoveHandlers(); // Set the signal handlers to not point to error_signals::Handler.
	StackTrace *GetStackTrace();
};

extern error_signals *error_thread_list;


#endif
