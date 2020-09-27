#ifndef __STACKTRACE_H
#define __STACKTRACE_H


class StackTrace
{
	public:	
	static const int CALL_STACK_SIZE = 200;
	const char *FileNames[CALL_STACK_SIZE];
	const char *FunctionNames[CALL_STACK_SIZE];
	int SavedLineNumbers[CALL_STACK_SIZE];	
	int LineNumberStack;
	void DisplayErrorMessage(int val);
	void DisplayErrorMessage(const char *message,int message_length);
	int ToString(char *output_buffer,int max_buffer_length);
	private:
	static void AppendString(const char *input,int input_length,char *output_buffer,int max_buffer_length,int &bytes_in_buffer);
	static void AppendString(const char *input,char *output_buffer,int max_buffer_length,int &bytes_in_buffer);
	static int Itoa(int value,char *buffer); // Convert an int to a string. Return the string length.
	static const char *basename(const char *filename); // Returns the file name without directory info.	
};
#endif
