#include "file_location.h"
file_location::file_location(datablock *folder)
{
	file_folder = folder == nullptr ? folder : folder->clone();
}

file_location::~file_location()
{
	datablock::dereference(&file_folder); 	
}

// Figures out the directory separator from the file_folder.
char file_location::directory_separator() 
{	
	char separator = '/';
	char search;
	int count;
	int length;
	int loop;
	char *data;
	
	if (file_folder != nullptr) {
		count = file_folder->length;
		data = file_folder->data;
		if ((count > 0) && (data != nullptr)) {
			for(loop=0;loop<count;loop++) {
				search = data[loop];
				if ((search == '/') || (search == '\\')) {
					separator = search;
					break;
				}
			}
		}
	}
	return separator;
}
