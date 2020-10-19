#include "datablock.h"
#include <string.h>
#include <inttypes.h>

datablock::datablock(int len)
{
	initialize(len);
}
datablock::datablock(datastring input)
{
	initialize(input.length);
	if (input.length > 0) {
		memcpy(data,input.data,input.length);		
	}
}
datablock::datablock(const char *input)
{
	size_t len;
	if (input == nullptr) {
		initialize(0);		
	} else {
		len = strlen(input);	
		initialize(len);
		memcpy(data,input,len);
		data[len] = 0;
	}
}
datablock::datablock(const char *input,int len)
{
	if ((input == nullptr) || (len <= 0)) {
		initialize(0);		
	} else {
		initialize(len);
		memcpy(data,input,len);
		data[len] = 0;
	}
}
datablock::~datablock()
{
	if (usage <= 1) {
		clear();
	} else {
		usage--;		
	}
}
void datablock::initialize(int len)
{	
	capacity = len+1;
	data = new char[capacity];
	length = len;
	data[len] = 0;
	null_terminated = true;
	usage = 1;
	return;
}
void datablock::clear()
{
	if (data != nullptr) {
		delete data;
		data = nullptr;
	}
	length = 0;
	capacity = 0;
	return;
}
void datablock::dereference(datablock **data)
{	
	if ((data != nullptr) && (*data != nullptr)) {
		if (--((*data)->usage) <=0) {
			delete *data;
		}
		*data = nullptr;
	}
	return;
}
datablock *datablock::clone()
{
	usage++;
	return this;
}
datablock &datablock::operator=(datastring input)
{
	length = 0;	
	return operator+=(input);
}
datablock &datablock::operator=(const char *input)
{
	length = 0;	
	return operator+=(input);
}
datablock &datablock::operator+=(datastring input)
{
	append(input.data,input.length);
	return *this;
}
void datablock::append(const char *input,int len)
{
	char *temp;
	int new_length;
	int new_capacity;
	if ((input != nullptr) && (len > 0)) {
		new_length = length + len;
		if (new_length >= capacity) {
			// Expand the string.
			new_capacity = (new_length << 1) + 1;
			temp = new char[new_capacity];
			if (length > 0) {
				memcpy(temp,data,length);
			}
			memcpy(temp+length,input,len);
			capacity = new_capacity;
			data = temp;			
		} else {
			memcpy(data+length,input,len);
		}
		length = new_length;
		null_terminated = true;
		data[length] = 0;
	}
	return;
}
datablock &datablock::operator+=(const char *input)
{	
	int len = (input == nullptr ? 0 : strlen(input));
	append(input,len);
	return *this;
}
datablock &datablock::operator=(int64_t input)
{
	length = 0;	
	return operator+=(input);
}
datablock &datablock::operator+=(int64_t input)
{
	char buffer[50];
	int len;	
	len = snprintf(buffer,50,"%" PRId64,input);
	append(buffer,len);
	return *this;
}
