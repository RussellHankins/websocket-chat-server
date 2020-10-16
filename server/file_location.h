#ifndef __FILE_LOCATION_H
#define __FILE_LOCATION_H
#include "datastring.h"
#include "datablock.h"

class file_location
{
	public:
	file_location(datablock *folder);
	~file_location();
	datablock *file_folder;
};
#endif
