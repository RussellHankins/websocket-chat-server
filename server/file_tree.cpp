#include "file_tree.h"
#include "stringbuilder.h"

file_tree::file_tree()
{
	location = nullptr;
	top_directory = nullptr;
}

file_tree::~file_tree()
{
	location = nullptr;
	if (top_directory != nullptr) {
		delete top_directory;
		top_directory = nullptr;
	}
}

void file_tree::read_directory(int64_t userid)
{
	char directory_separator[2];
	datastring separator;
	stringbuilder root_folder;	
	if (location == nullptr) {
		return;
	}
	directory_separator[0] = location->directory_separator();
	directory_separator[1] = 0;
	separator.data = directory_separator;
	separator.length = 1;
	separator.null_terminated = true;
	root_folder += *location->file_folder;
	if (location->file_folder->substr(-1,1)!=directory_separator[0]) {
		root_folder += separator;
	}
	root_folder += userid;
	datablock root(root_folder.length());
	root_folder.tostring(root.data);
	if ((top_directory != nullptr) && (root != *top_directory->name)) {
		delete top_directory;
		top_directory = nullptr;
	}	
	if (top_directory == nullptr) {	
		top_directory = new file_directory();		
		top_directory->name = root.clone();
	}
	top_directory->read_directory(directory_separator[0],true);
}
