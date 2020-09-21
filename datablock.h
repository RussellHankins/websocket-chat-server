#ifndef __DATABLOCK_H
#define __DATABLOCK_H
#include <stdio.h>
#include <cstdint>
#include "datastring.h"

class datablock:public datastring
{
	public:
	datablock(int len); // Makes a garbage string of len bytes. Allocates capacity to len+1.
	datablock(const char *input); // Makes a deep copy of input.
	datablock(const char *input,int len); // Makes a deep copy of input.
	datablock(datastring input); // Makes a deep copy of input.
	~datablock();	
	int capacity; // The total number of bytes in data[].
	int usage; // The number of pointers pointing to this object.
	void initialize(int len); 
	void clear(); // Deallocate data[] and set capacity,length to 0.	
	// Disconnect from this data block. 
	// Decrements usage. Deletes if usage <=0.
	static void dereference(datablock **data); 	
	datablock *clone(); // Make a copy of this object by incrementing usage.
	datablock &operator=(datastring input);
	datablock &operator=(const char *input);
	datablock &operator+=(datastring input);
	datablock &operator+=(const char *input);
	datablock &operator=(int64_t input);
	datablock &operator+=(int64_t input);
	void append(const char *input,int len);
};
#endif
