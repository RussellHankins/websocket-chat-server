#include "file_tree.h"

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
