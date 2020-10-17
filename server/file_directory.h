#ifndef __FILE_DIRECTORY_H
#define __FILE_DIRECTORY_H
#include "biglist.h"
#include "datablock.h"
#include "file_location.h"
#include "file_item.h"
// A file_directory is a directory and which files and subdirectories are in it.
class file_directory
{
	public:
	file_directory();
	~file_directory();
	datablock *name;
	file_directory *parent;
	biglist<file_directory *> *subdirectories;
	biglist<file_item *> *files;
	
};
#endif
