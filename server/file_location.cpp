#include "file_location.h"
file_location::file_location(datablock *folder)
{
	file_folder = folder == nullptr ? folder : folder->clone();
}

file_location::~file_location()
{
	datablock::dereference(&file_folder); 	
}
