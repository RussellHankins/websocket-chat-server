#include "message.h"
#include <string.h>
#include <libwebsockets.h>

message::message()
{
	initialize();
}
message::~message()
{
	clear();
}
void message::initialize()
{
	buffer = nullptr;
	usage = 1;
	return;
}
void message::clear()
{
	datablock::dereference(&buffer);
	actual_message.data = nullptr;
	actual_message.length = 0;
	return;
}
bool message::set(const char *in)
{
	int length;
	length = (in == nullptr ? 0 : strlen(in));
	return set(in, length);
}
bool message::set(datastring item)
{
	return set(item.data, item.length);
}
void message::setcapacity(int capacity)
{
	int buffersize = LWS_SEND_BUFFER_PRE_PADDING + capacity + 1 + LWS_SEND_BUFFER_POST_PADDING;
	if ((buffer != nullptr) && (buffer->capacity < buffersize)) {
		delete buffer;
		buffer = nullptr;
	}
	if (buffer == nullptr) {
		buffer = new datablock(buffersize);
	}
	actual_message.data = buffer->data + LWS_SEND_BUFFER_PRE_PADDING;
	actual_message.data[capacity] = 0;
	actual_message.length = capacity;
	actual_message.null_terminated = true;	
	return;
}

bool message::set(const char *in, int len)
{
	bool success = true;
	int buffersize = LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING;
	if ((buffer != nullptr) && (buffer->capacity < buffersize)) {
		delete buffer;
		buffer = nullptr;
	}
	if (buffer == nullptr) {
		buffer = new datablock(buffersize);
	}
	if ((buffer != nullptr) && (buffer->data != nullptr)) {
		memcpy(buffer->data+LWS_SEND_BUFFER_PRE_PADDING,in,len);		
		actual_message.data = buffer->data + LWS_SEND_BUFFER_PRE_PADDING;
		actual_message.length = len;
	} else {
		actual_message.data = nullptr;
		actual_message.length = 0; // Memory error.
		success = false;
	}
	actual_message.null_terminated = false;	
	
	return success;
}
message *message::clone()
{
	usage++;
	return this;
}
void message::dereference(message **item)
{
	if ((item != nullptr) && (*item != nullptr)) {
		if (--(*item)->usage <=0) {
			delete *item;
		}
		*item = nullptr;
	}
	return;
}

message &message::operator=(stringbuilder &item)
{
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
	message_length = item.length();
	buffersize = LWS_SEND_BUFFER_PRE_PADDING + message_length + LWS_SEND_BUFFER_POST_PADDING+1;
	if ((buffer != nullptr) && (buffer->capacity <= buffersize)) {
		delete buffer;
		buffer = nullptr;
	}
	if (buffer == nullptr) {
		buffer = new datablock(buffersize);
	}
	pos = buffer->data+LWS_SEND_BUFFER_PRE_PADDING;
	item.tostring(pos);
	actual_message.data = pos;
	actual_message.length = message_length;
	actual_message.null_terminated = true;
	return *this;
}
