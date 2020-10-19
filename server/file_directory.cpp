#include "file_directory.h"
#include "stringbuilder.h"

file_directory::file_directory()
{
	subdirectories = nullptr;
	parent = nullptr;
	files = nullptr;
	name = nullptr;
}

file_directory::~file_directory()
{
	datablock::dereference(&name);
	parent = nullptr;
	if (files != nullptr) {
		biglist_iterator<file_item *>file_loop(files);
		while (!file_loop.eof()) {
			delete file_loop.item;
			file_loop.row->used = false;
			file_loop.movenext();
		}
		delete files;
		files = nullptr;
	}
	if (subdirectories != nullptr) {
		biglist_iterator<file_directory *>sub_loop(subdirectories);
		while (!sub_loop.eof()) {
			delete sub_loop.item;
			sub_loop.row->used = false;
			sub_loop.movenext();
		}
		delete subdirectories;
		subdirectories = nullptr;
	}	
}

stringbuilder *file_directory::getfullpath(datastring directory_separator)
{
	stringbuilder *output;
	if (parent == nullptr) {
		output = new stringbuilder();
		*output += *name;
	} else {
		output = parent->getfullpath(directory_separator);
	}
	// Use recursion.
	output = parent->getfullpath(directory_separator);
	if (output->last_character() != directory_separator.data[0]) {
		*output += directory_separator;
	}
	*output += *name;
	return output;
}

void file_directory::read_directory(char directory_separator,bool recurse_subdirectories)
{
	datablock *directory_name;	
	stringbuilder *directory_built;
	datastring directory_separator_string;
	
	directory_separator_string.data = &directory_separator;
	directory_separator_string.length = 1;
	directory_separator_string.null_terminated = false;
	if (parent == nullptr) {
		directory_name = name->clone();		
	} else {
		directory_built = getfullpath(directory_separator_string);
		directory_name = new datablock(directory_built->length());
		directory_built->tostring(directory_name->data);
		delete directory_built;
	}
	
	// not finished.
	
	delete directory_name;
}
