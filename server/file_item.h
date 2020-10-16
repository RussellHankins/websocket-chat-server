#ifndef __FILE_ITEM_H
#define __FILE_ITEM_H
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include "concurrent_queue.h"
#include "datablock.h"
#include "message.h"

class file_item
{
	public:
	file_item(datablock *name,int64_t size);
	~file_item();
	void clear();
	datablock *filename;
	time_t last_access;
	FILE *file;
	int64_t filesize;
	int64_t position;
	void set_last_access();
	concurrent_queue<message *> *messages;
	void seek(int64_t spot); // Moves the read pointer to that spot if necessary.
	size_t read(datastring buffer);
	size_t write(datastring buffer);
	void close_if_not_busy();
	bool open(); // Open the file if it's not already open. Sets last_access time.
};
#endif
