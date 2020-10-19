#ifndef __FILE_TREE_H
#define __FILE_TREE_H
#include "file_location.h"
#include "file_directory.h"
#include <cstdint>
// file_tree combines a file_location and a top level file_directory.
class file_tree
{
	public:
	file_tree();
	~file_tree();
	file_location *location;
	file_directory *top_directory;
	void read_directory(int64_t userid);
};
#endif
