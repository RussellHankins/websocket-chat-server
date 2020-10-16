#include "file_item.h"
file_item::file_item(datablock *name,int64_t size)
{
	filename = (name == nullptr ? name : name->clone());
	messages = new concurrent_queue<message *>();
	filesize = size;
	position = 0;
}

file_item::~file_item()
{
	message *message_to_delete;
	datablock::dereference(&filename);
	if (messages != nullptr) {
		while (!messages->empty()) {
			if (messages->dequeue(message_to_delete)) {
				message::dereference(&message_to_delete);
			}
		}
		delete messages;	
		messages = nullptr;
	}
}

void file_item::set_last_access()
{
	time(&last_access);
}

void file_item::close_if_not_busy()
{
	time_t now;
	if (file != nullptr) {
		time(&now);
		if (now - last_access > 20) {
			// Close the file.
			fclose(file);
			file = nullptr;
		}
	}
	return;
}

bool file_item::open()
{
	if (file == nullptr) {
		file = fopen(filename->data,"a+");
		if (file != nullptr) {
			set_last_access();
			position = ftell(file);
			filesize = position;
		}
	} else {
		set_last_access();
	}
	return file != nullptr;
}

void file_item::seek(int64_t spot)
{
	if ((spot != position) && (file != nullptr)) {
		if (0 == fseek(file,spot,SEEK_SET)) {
			position = spot;
		}
	}
	return;
}

size_t file_item::read(datastring buffer)
{
	size_t bytes_read = 0;
	if (file != nullptr) {
		bytes_read = fread(buffer.data,1,buffer.length,file);
		position += bytes_read;
	}
	return bytes_read;
}

size_t file_item::write(datastring buffer)
{
	size_t bytes_read = 0;
	if (file != nullptr) {
		bytes_read = fwrite(buffer.data,1,buffer.length,file);
		fflush(file);
		position += bytes_read;
		if (position > filesize) {
			filesize = position; // Made the file longer.
		}
	}
	return bytes_read;
}
