#include "StackTrace.h"
#include <csignal>
#include <string.h>
#include <stdio.h>


#define cstr(x) x,sizeof(x)-1

void StackTrace::AppendString(const char *input,int input_length,char *output_buffer,int max_buffer_length,int &bytes_in_buffer)
{
	if ((input_length+bytes_in_buffer < max_buffer_length) && (output_buffer != nullptr))
	{
		memcpy(output_buffer+bytes_in_buffer,input,input_length);		
	}
	bytes_in_buffer += input_length;
	return;
}
void StackTrace::AppendString(const char *input,char *output_buffer,int max_buffer_length,int &bytes_in_buffer)
{
	char ch;
	if (output_buffer == nullptr) {
		while (true) {
			ch = *(input++);
			if (ch==0) {
				break;
			}
			bytes_in_buffer++;
		}
	} else {
		while (true) {
			ch = *(input++);
			if (ch == 0) {
				break;
			}
			if (bytes_in_buffer < max_buffer_length) {
				output_buffer[bytes_in_buffer] = ch;
			}
			bytes_in_buffer++;
		}
	}
	return;
}
int StackTrace::ToString(char *output_buffer,int max_buffer_length)
{
	// Converts the stack trace to a string. Returns the length of the data. 
	// If output_buffer is nullptr or too small, the actual needed length is returned.
	int lineNumberIndex;
	const char *file_name;
	const char *function_name;
	char buffer[40];
	int length;
	int bytes_in_buffer = 0;
	int lineNumber;	
	
	// Show the stack trace:	
	if (LineNumberStack < 0)
	{
		LineNumberStack = 0;
	}
	if (LineNumberStack > CALL_STACK_SIZE)
	{
		LineNumberStack = CALL_STACK_SIZE;
	}
	for(lineNumberIndex=0;lineNumberIndex<LineNumberStack;lineNumberIndex++)
	{
		lineNumber = SavedLineNumbers[lineNumberIndex];
		if (lineNumber != 0)
		{
			AppendString(cstr("\n"),output_buffer,max_buffer_length,bytes_in_buffer);
			file_name = FileNames[lineNumberIndex];
			if (file_name !=nullptr) {
				AppendString(cstr(" in file "),output_buffer,max_buffer_length,bytes_in_buffer);
				file_name = basename(file_name);
				AppendString(file_name,output_buffer,max_buffer_length,bytes_in_buffer);
			}
			function_name = FunctionNames[lineNumberIndex];
			if (function_name != nullptr) {
				AppendString(cstr(" in function "),output_buffer,max_buffer_length,bytes_in_buffer);
				AppendString(function_name,output_buffer,max_buffer_length,bytes_in_buffer);
			}
			AppendString(cstr(" in line "),output_buffer,max_buffer_length,bytes_in_buffer);
			length = Itoa(lineNumber,buffer);
			AppendString(buffer,length,output_buffer,max_buffer_length,bytes_in_buffer);
		}
	}
	if (buffer != nullptr) {
		buffer[bytes_in_buffer < max_buffer_length ? bytes_in_buffer : max_buffer_length-1] = 0;	
	}
	return bytes_in_buffer;
}
void StackTrace::DisplayErrorMessage(const char *message,int message_length)
{
	int lineNumberIndex;
	const char *file_name;
	const char *function_name;
	char buffer[40];
	int length;
	int lineNumber;	
	
	if ((message == nullptr) || (message_length <=0)) {
		printf("Error");		
	} else {
		printf("%s",message);		
	}
	// Show the stack trace:	
	if (LineNumberStack < 0)
	{
		LineNumberStack = 0;
	}
	if (LineNumberStack > CALL_STACK_SIZE)
	{
		LineNumberStack = CALL_STACK_SIZE;
	}
	for(lineNumberIndex=0;lineNumberIndex<LineNumberStack;lineNumberIndex++)
	{
		lineNumber = SavedLineNumbers[lineNumberIndex];
		if (lineNumber != 0)
		{
			printf("\n");
			file_name = FileNames[lineNumberIndex];
			if (file_name != nullptr) {
				file_name = basename(file_name);
				printf(" in file %s",file_name);				
			}
			function_name = FunctionNames[lineNumberIndex];
			if (function_name != nullptr) {
				printf(" in function %s",function_name);
			}
			printf(" in line %d",lineNumber);
		}
	}
	printf("\n");
	return;
}

void StackTrace::DisplayErrorMessage(int val)
{
	const char *error;	
	int error_length;
	// Show the error description based on the signal number:
	switch (val) {
		case SIGFPE: {
		  error = "Division by zero error";
		  error_length = 22;
		  break; }
		case SIGILL: {
		  error = "Illegal instruction error";
		  error_length = 25;
		  break; }
		case SIGSEGV: {
		  error = "Bad memory read/write error";
		  error_length = 27;
		  break; }
		case SIGBUS: {
		  error = "Access misalligned memory or non-existent memory error";
		  error_length = 54;
		  break; }
		default: {
		  error = "Unknown signal error";
		  error_length = 20;
		  break; }
	}
	DisplayErrorMessage(error,error_length);	
	return;
}
int StackTrace::Itoa(int value,char *buffer)
{
	char *pos = buffer;
	int divisor = 1000000000;
	int digit;
	if (value < 0)
	{
		*(pos++) = '-';
		value = -value;
	}
	if (value == 0)
	{
		*(pos++) = '0';
	} else
	{
		while (divisor > value)
		{
			divisor /= 10;
		}
		while(divisor > 0)
		{
			digit = value / divisor;
			value -= digit * divisor;
			*(pos++) = digit + '0';
			divisor /= 10;
		}
	}
	*pos = 0;
	return pos - buffer;
}

const char *StackTrace::basename(const char *filename)
{
	const char *loop;
	const char *basename;
	char ch;
	basename = filename;
	loop = filename;
	do {
		ch = *(loop++);
		if ((ch == '/') || (ch == '\\')) {
			basename = loop;
		}	
	} while (ch != 0);
	return basename;
}
