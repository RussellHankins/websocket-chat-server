#include "file_dir_info.h"

#if defined _WIN32 || defined _WIN64
#include <windows.h>
#include <io.h>
#include <string.h>
#include <direct.h>
#endif

#ifdef __unix__
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#endif

file_dir_info::file_dir_info()
{
	name = nullptr;
	next = nullptr;
}

file_dir_info::~file_dir_info()
{
	file_dir_info *loop;
	file_dir_info *item;
	datablock::dereference(&name);
	// Iterative delete children so the stack doesn't crash.
	loop = next;
	while (loop != nullptr) {
		item = loop;
		loop = loop->next;
		item->next = nullptr;
		delete item;
	}
}

int file_dir_info::file_count()
{
	file_dir_info *loop = this;
	int count = 0;
	while (loop != nullptr) {
		if (!loop->is_directory) {
			count++;
		}
		loop = loop->next;
	}
	return count;
}

int file_dir_info::directory_count()
{
	file_dir_info *loop = this;
	int count = 0;
	while (loop != nullptr) {
		if (loop->is_directory) {
			count++;
		}
		loop = loop->next;
	}
	return count;
}

void file_dir_info::delete_file(datastring *file)
{
	datablock *temp;
	if (file->null_terminated) {
		remove(file->data);
	} else {
		temp = new datablock(*file);
		remove(temp->data);
		delete temp;
	}
}

// ****************************** LINUX *****************************
#ifdef __unix__
void file_dir_info::create_directory(datastring *directory)
{
	datablock *temp;
	if (directory->null_terminated) {
		mkdir(directory->data,0777);
	} else {
		temp = new datablock(*directory);
		mkdir(temp->data,0777);
		delete temp;
	}
}

void file_dir_info::remove_directory(datastring *directory)
{
	datablock *temp;
	if (directory->null_terminated) {
		rmdir(directory->data);
	} else {
		temp = new datablock(*directory);
		rmdir(temp->data);
		delete temp;
	}
}

file_dir_info *file_dir_info::read_directory(datablock *directory
	,datastring directory_separator,bool &directory_exists)
{
	// This function is operating system specific.
	file_dir_info *listing = nullptr;
	file_dir_info *listing_end;
	file_dir_info *item;
	char dot;
	DIR *dir;
	struct dirent ent;
	struct dirent *result;
	struct stat file_info;
	datablock temp_file(1024);
	datablock *temp;
	
	// Open directory
	if (directory->null_terminated) {
		dir = opendir(directory->data);	
	} else {
		temp = new datablock(*directory);
		dir = opendir(temp->data);	
		delete temp;
	}	
	// Loop through directory.
	if (dir == nullptr) {
		directory_exists = false;
	} else {		
		directory_exists = true;
		temp_file = *directory;
		while (true) {
			// Read the next directory item (linux).
			readdir_r(dir,&ent,&result);
			if (result == nullptr) {
				break;
			}			
			// Look for . or .. directories to skip.
			if (ent.d_name[0] == '.') {
				dot = ent.d_name[1];
				if (dot == 0) {
					continue;
				}
				if (dot == '.') {
					if (ent.d_name[2] == 0) {
						continue;
					}
				}
			}
			
			item = new file_dir_info();			
			item->name = new datablock(ent.d_name);			
			// Get file info.
			temp_file.length = directory->length;
			temp_file += directory_separator;
			temp_file += ent.d_name;
			stat(temp_file.data,&file_info);
			item->length = file_info.st_size;
			item->is_directory = (file_info.st_mode & S_IFDIR) != 0;
			item->next = nullptr;
			if (listing == nullptr) {
				listing = item;				
			} else {
				listing_end->next = item;	
			}
			listing_end = item;
		}
		closedir(dir);
	}
	return listing;
}
#endif

// ****************************** WINDOWS *****************************
#if defined _WIN32 || defined _WIN64
void file_dir_info::create_directory(datastring *directory)
{
	datablock *temp;
	if (directory->null_terminated) {
		_mkdir(directory->data);
	} else {
		temp = new datablock(*directory);
		_mkdir(temp->data);
		delete temp;
	}
}

void file_dir_info::remove_directory(datastring *directory)
{
	datablock *temp;
	if (directory->null_terminated) {
		DeleteDirectory(directory->data);
	} else {
		temp = new datablock(*directory);
		DeleteDirectory(temp->data);
		delete temp;
	}
}

file_dir_info *file_dir_info::read_directory(datablock *directory
	,datastring directory_separator,bool &directory_exists)
{
	// This function is operating system specific.
	file_dir_info *listing = nullptr;
	file_dir_info *listing_end;
	file_dir_info *item;
	char dot;
	struct __finddata64_t ent;
	intptr_t dir;
	bool skip;
	datablock *temp;
	// Open directory
	if (directory->null_terminated) {
		dir = _findfirst64(directory->data,&ent);	
	} else {
		temp = new datablock(*directory);
		dir = _findfirst64(temp->data,&ent);
		delete temp;
	}
	// Loop through directory.
	if (dir == -1) {
		directory_exists = false;
	} else {		
		directory_exists = true;
		while (true) {
			// Look for . or .. directories to skip.
			skip = false;
			if (ent.name[0] == '.') {
				dot = ent.name[1];
				if (dot == 0) {
					skip = true;
				}
				if (dot == '.') {
					if (ent.name[2] == 0) {
						skip = true;
					}
				}
			}
			if (!skip) 
			{			
				item = new file_dir_info();			
				item->name = new datablock(ent.name);			
				item->length = ent.size;
				item->is_directory = (ent.attrib & _A_SUBDIR) != 0;
				item->next = nullptr;
				if (listing == nullptr) {
					listing = item;				
				} else {
					listing_end->next = item;	
				}
				listing_end = item;
			}
			// Read the next directory entry.
			if (_findnext64(dir,&ent) == -1) {
				break;
			}
		}
		_findclose(dir);
	}
	return listing;
}
#endif
