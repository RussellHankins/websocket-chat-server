#ifndef __FILE_LOCATION_H
#define __FILE_LOCATION_H
#include "datastring.h"
#include "datablock.h"
// A file_location is a path to the root directory of a hard drive somewhere.
// It is specified as a command line parameter when the program starts.
class file_location
{
	public:
	file_location(datablock *folder);
	~file_location();
	datablock *file_folder;	
	char directory_separator(); // Figures out the directory separator from the file_folder.
};
#endif
