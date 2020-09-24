#ifndef __DEBUG_H
#define __DEBUG_H

#include "error_signals.h"

// This class is so that if the program crashes, a stack trace will be displayed
// just before the program crashes.
// It uses error_signals.h and StackTrace.h

// Example:
// Debug debug(__FILE__,__func__,__LINE__);
// debug = __LINE__;
class Debug
{
	public:	
	int LineNumber;	
	Debug(int lineNumber);
	Debug(const char *fileName,const char *functionName,int lineNumber);
	~Debug();
	void SaveLineNumbers(); // Call SaveLineNumbers before rewinding the stack because Thread.LineNumbers points to LineNumber in other Debug objects.
	void operator=(int lineNumber) { LineNumber = lineNumber; };
	private:
	error_signals *_thread;	
};
#endif
