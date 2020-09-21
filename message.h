#ifndef __MESSAGE_H
#define __MESSAGE_H
#include <stdio.h>
#include "datablock.h"
#include "stringbuilder.h"
// This is a message to send to the server.
class message
{
	public:
	datablock *buffer;
	datastring actual_message;
	int usage;
	message();
	~message();		
	void initialize();
	void setcapacity(int capacity);
	message &operator=(stringbuilder &item);
	void clear();
	bool set(const char *in);
	bool set(const char *in, int len);
	bool set(datastring item);
	message *clone();
	static void dereference(message **item);
};
#endif

