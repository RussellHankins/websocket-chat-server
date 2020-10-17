#include "file_directory.h"
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
