#ifndef __FILE_DIRECTORY_H
#define __FILE_DIRECTORY_H
#include "biglist.h"
#include "datablock.h"
#include "file_location.h"
#include "file_item.h"
#include "stringbuilder.h"
class file_directory
{
	public:
	file_directory();
	~file_directory();
	datablock *name;
	file_directory *parent;
	bool directory_exists;
	biglist<file_directory *> *subdirectories;
	biglist<file_item *> *files;
	void read_directory(char directory_separator,bool recurse_subdirectories);
	void create_directory();
	stringbuilder *getfullpath(datastring directory_separator);
};
#endif
