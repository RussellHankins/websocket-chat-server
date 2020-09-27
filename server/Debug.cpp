#include "Debug.h"


Debug::Debug(int lineNumber)
{
	pid_t thread_id;
	
	LineNumber = lineNumber;
	thread_id = error_signals::GetThreadID();
	_thread = error_signals::GetThread(thread_id);
	if (_thread != nullptr)
	{
		// Store a pointer to the current line number in the thread line number stack.
		if (_thread->LineNumberStack < error_signals::CALL_STACK_SIZE)
		{
			_thread->LineNumbers[_thread->LineNumberStack] = &LineNumber;
			_thread->FileNames[_thread->LineNumberStack] = nullptr;
			_thread->FunctionNames[_thread->LineNumberStack++] = nullptr;
		}
	}
}
Debug::Debug(const char *fileName,const char *functionName,int lineNumber)
{
	pid_t thread_id;
	
	LineNumber = lineNumber;	
	thread_id = error_signals::GetThreadID();
	_thread = error_signals::GetThread(thread_id);
	if (_thread != nullptr)
	{
		// Store a pointer to the current line number in the thread line number stack.
		if (_thread->LineNumberStack < error_signals::CALL_STACK_SIZE)
		{
			_thread->LineNumbers[_thread->LineNumberStack] = &LineNumber;
			_thread->FileNames[_thread->LineNumberStack] = fileName;
			_thread->FunctionNames[_thread->LineNumberStack++] = functionName;
			
		}
	}
}
void Debug::SaveLineNumbers()
{
	if (_thread != nullptr) {
		_thread->SaveLineNumbers();
	}
	return;
}
Debug::~Debug()
{
	if (_thread != nullptr)
	{
		// Pop the line number off the line number stack.
		
		if (_thread->LineNumberStack > 0)
		{
			_thread->LineNumberStack--;
		}
	}
}
