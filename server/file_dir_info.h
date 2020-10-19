#ifndef __FILE_DIR_INFO_H
#define __FILE_DIR_INFO_H
#include "datablock.h"
// A file_directory is a directory and which files and subdirectories are in it.
class file_dir_info
{
	public:
	file_dir_info();
	~file_dir_info();
	datablock *name;
	int64_t length;
	bool is_directory;
	file_dir_info *next;
	static void delete_file(datastring *file);
	int file_count();
	int directory_count();
	// The following functions contain code specific to __unix__ and _WIN32 and _WIN64
	static file_dir_info *read_directory(datablock *directory
		,datastring directory_separator,bool &directory_exists);	
	static void create_directory(datastring *directory);
	static void remove_directory(datastring *directory);	
};
#endif
