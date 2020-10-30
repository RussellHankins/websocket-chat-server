#include "stringbuilder.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

stringbuilder_node::stringbuilder_node()
{
	// This constructor is only used for fast_nodes.
}
stringbuilder_node::stringbuilder_node(datastring node,bool should_delete)
{
	data.item = node;
	need_to_delete = should_delete;
	is_number = false;
}
stringbuilder_node::stringbuilder_node(int64_t value)
{
	data.number = value;
	need_to_delete = false;
	is_number = true;
}
void stringbuilder_node::clear()
{
	if ((!is_number) && (data.item.data != nullptr)) {
		if (need_to_delete) {
			delete data.item.data;
		}
		data.item.data = nullptr;
	}
	return;	
}

	
stringbuilder::stringbuilder()
{
	index = 0;
}
stringbuilder::~stringbuilder()
{
	clear();
}

stringbuilder &stringbuilder::addparameter(int item)
{
	// Appends the number, then a comma.
	return addparameter((int64_t)item);
}

stringbuilder &stringbuilder::addparameter(int64_t item)
{
	// Appends the number, then a comma.
	operator+=(item);
	operator+=(",");
	return *this;
}

stringbuilder &stringbuilder::addparameter(datastring *item)
{
	if ((item == nullptr) || (item->length <= 0) || (item->data == nullptr)) {
		operator+=("0,");
	} else {
		addparameter(*item);
	}
	return *this;
}

stringbuilder &stringbuilder::addparameter(datastring item)
{
	// Appends the length first, then a comma, then the data.
	operator+=(item.length);
	operator+=(",");
	append(item,false);
	return *this;
}

stringbuilder &stringbuilder::operator+=(const char *item)
{
	datastring temp;
	temp.data = (char *)item;
	temp.length = item == nullptr ? 0 : strlen(item);
	temp.null_terminated = true;	
	append(temp, false);
	return *this;
}
stringbuilder &stringbuilder::operator+=(datastring item)
{
	append(item, false);
	return *this;
}
stringbuilder &stringbuilder::operator+=(int item)
{
	return append((int64_t)item);
}
stringbuilder &stringbuilder::operator+=(int64_t item)
{
	return append(item);
}
stringbuilder &stringbuilder::append(int64_t item)
{
	auto node = nodes[index];
	node->used = true;
	if (index < FAST_NODE_COUNT) {
		node->item = fast_nodes + index;
		node->item->data.number = item;
		node->item->is_number = true;
		node->item->need_to_delete = false;
	} else {
		node->item = new stringbuilder_node(item);
	}
	index++;
	return *this;
}
stringbuilder &stringbuilder::append(datastring item, bool need_to_delete)
{	
	auto node = nodes[index];
	node->used = true;
	if (index < FAST_NODE_COUNT) {
		node->item = fast_nodes + index;
		node->item->data.item = item;
		node->item->is_number = false;
		node->item->need_to_delete = need_to_delete;
	} else {
		node->item = new stringbuilder_node(item,need_to_delete);
	}
	index++;
	return *this;
}
stringbuilder &stringbuilder::add_encodeURIComponent(datastring item)
{
	const char *not_escaped = "-_.!!*'()";
	const char *hex = "0123456789ABCDEF";
	int source_length = item.length;
	int target_length = item.length;
	int loop;
	char *result;
	datastring output;
	char ch;
	for(loop=0;loop<source_length;loop++) {
		ch = item.data[loop];
		if (((ch < '0') || (ch > '9')) // Not a number.
		&& ((ch < 'A') || (ch > 'Z')) // Not an uppercase letter.
		&& ((ch < 'a') || (ch > 'z')) // Not a lowercase letter.
		&& (ch != ' ') // Not a space.
		&& (nullptr == strchr(not_escaped,ch))) { // Not a symbol.
			// Need to escape.			
			target_length += 2;
		}
	}
	result = new char[target_length];	
	target_length = 0;
	for(loop=0;loop<source_length;loop++) {
		ch = item.data[loop];
		if (((ch < '0') || (ch > '9')) // Not a number.
		&& ((ch < 'A') || (ch > 'Z')) // Not an uppercase letter.
		&& ((ch < 'a') || (ch > 'z')) // Not a lowercase letter.
		&& (nullptr == strchr(not_escaped,ch))) { // Not a symbol.
			// Need to escape.
			if (ch == ' ') {
				result[target_length++] = '+';
			} else {
				result[target_length++] = '%';
				result[target_length++] = hex[ch >> 4];
				result[target_length++] = hex[ch & 15];
			}
		} else {
			result[target_length++] = ch;
		}
	}
	output.data = result;
	output.length = target_length;
	output.null_terminated = false;
	append(output,true); // The class destructor will eventually delete result;
	return *this;
}
int stringbuilder::length()
{
	int loop;
	int total = 0;
	int64_t number;
	for(loop=0;loop<index;loop++) {
		auto node = nodes[loop];
		if (node->item->is_number) {
			// Figure out the length of the number when converted to base 10.
			number = node->item->data.number;
			if (number < 0) {
				// Minus sign.
				number = -number;
				total++;
			}
			total++; // First digit.
			// Count other digits.
			while (number >= 10000) {
				total += 4;
				number /= 10000;
			}
			while (number >= 10) {
				total++;
				number /= 10;
			}
		} else {
			total += node->item->data.item.length;
		}
	}
	return total;
}
void stringbuilder::clear()
{
	// Clears the list and deletes any allocated items.	
	int loop;
	for(loop=0;loop<index;loop++) {
		auto node = nodes[loop];
		node->item->clear();
		if (loop >= FAST_NODE_COUNT) {
			delete node->item;
		}
		node->item = nullptr;
		node->used = false;
	}
	index = 0;	
	return;
}
void stringbuilder::print()
{
	biglist<stringbuilder_node *>nodes;
	biglist_iterator<stringbuilder_node *>loop(&nodes);
	while(!loop.eof()) {
		if (loop.item->is_number) {
			printf("%" PRId64,loop.item->data.number);
		} else {
			loop.item->data.item.print();
		}
		loop.movenext();
	}
	return;
}
void stringbuilder::println()
{
	print();
	printf("\n");
}
int stringbuilder::tostring(char *buffer)
{
	// Save this string to a buffer.
	// It assumes that the buffer was allocated already
	// by calling the length() function.
	// Be sure to allow an extra byte for the end of line.
	int message_length;
	int buffersize;
	char *pos;
	int node_loop;
	int item_length;
	int node_count;
	int64_t number;
	int64_t divisor;
	int64_t remainder;
	bool showzero;
	
	pos = buffer;
	node_count = index;
	for(node_loop=0;node_loop<node_count;node_loop++) {
		auto node = nodes[node_loop];
		auto node_item = node->item;
		if (node_item->is_number) {
			number = node->item->data.number;
			if (number < 0) {
				number = -number;
				*(pos++) = '-';
			}
			divisor = 1000000000000000000L;
			showzero = false;
			while (true) {
				remainder = number / divisor;
				if ((showzero) || (remainder > 0) || (divisor == 1)) {
					*(pos++) = (char)remainder + '0';
					showzero = true;
					number -= divisor * remainder;
				}
				if (divisor == 1) {
					break;
				}
				divisor /= 10;
			}
		} else {
			item_length = node_item->data.item.length;
			if (item_length > 0) {
				memcpy(pos,node_item->data.item.data,item_length);
				pos+=item_length;
			}
		}
	}
	*pos = 0;
	return pos - buffer;
}
char stringbuilder::last_character()
{
	// This function is helpful for file/directory names.
	stringbuilder_node *loop;
	int length;
	if (index == 0) {
		return 0;
	}
	if (index < FAST_NODE_COUNT) {
		loop = fast_nodes + (index-1);
	} else {		
		biglist_iterator<stringbuilder_node *>node_loop(&nodes);
		while(!node_loop.eof()) {
			loop = node_loop.item;
			node_loop.movenext();
		}
	}
	if (loop->is_number) {
		return (loop->data.number % 10)+48;
	}
	length = loop->data.item.length;
	return length == 0 ? 0 : loop->data.item.data[length-1];
}
