#ifndef __STRINGBUILDER_H
#define __STRINGBUILDER_H
#include "datastring.h"
#include "biglist.h"
#include <cstdint>
// stringbuilder is used to concatenate strings together.
// The strings are not actually concatenated until you store
// the stringbuilder into a datablock or a message.
// Numbers can be appended to the string too.
// When numbers are appended, the number is stored. 
// It's converted to a string when you call tostring().

union stringbuilder_data
{
	datastring item;	
	int64_t number; // If is_number is true, use this number instead of item.
};
	
class stringbuilder_node
{
	public:
	bool need_to_delete; // Should item.data be deleted in the clear() function?
	bool is_number; // Is this item a number?
	stringbuilder_data data;	
	stringbuilder_node(); // This constructor is only used for fast_nodes.
	stringbuilder_node(datastring node,bool should_delete);
	stringbuilder_node(int64_t value);	
	void clear();	
};
class stringbuilder
{
	public:
	stringbuilder();
	~stringbuilder();
	int index;
	biglist<stringbuilder_node *>nodes;
	stringbuilder &operator+=(const char *item);
	stringbuilder &operator+=(datastring item);	
	stringbuilder &operator+=(int item);
	stringbuilder &operator+=(int64_t item);
	stringbuilder &addparameter(int item); // Adds a number, then a comma.
	stringbuilder &addparameter(int64_t item); // Adds a number, then a comma.
	stringbuilder &addparameter(datastring item); // Appends the length first, then a comma, then the data.
	stringbuilder &addparameter(datastring *item); // Appends the length first, then a comma, then the data.
	stringbuilder &append(datastring item, bool need_to_delete);
	stringbuilder &append(int64_t item);
	void print();
	void println();
	int length(); // Returns the total number of bytes in this stringbuilder.
	void clear(); // Clears the list and deletes any allocated items.
	int tostring(char *buffer); // Save this string to a buffer. Returns the number of bytes.
	private:
	// fast_nodes are used to avoid allocating memory.
	static const int FAST_NODE_COUNT = 40;
	stringbuilder_node fast_nodes[FAST_NODE_COUNT];
};
#endif
