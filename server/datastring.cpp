#include "datastring.h"
#include <string.h>
#include <stdio.h>
#include "sha256.h"

datastring datastring::substr(int start)
{
	return substr(start,length-start);
}
datastring datastring::substr(int start,int len)
{
	datastring output;
	
	if (start < 0) {
		start += length;
	}
	
	if ((start < 0) || (start >= length)) {
		output.data = nullptr;
		output.length = 0;
		output.null_terminated = false;
	} else {
		if (start + len > length) {
			len = length - start;
		}
		output.data = data+start;
		output.length = len;
		output.null_terminated == null_terminated;
		if (start + len != length) {
			output.null_terminated = false;
		}		
	}	
	return output;
}
datastring &datastring::operator=(const char *item)
{
	data = (char *)item;
	length = strlen(item);
	null_terminated = true;
	return *this;	
}
int datastring::compare(char item)
{
	char first;
	if ((data == nullptr) || (length == 0)) {
		return item == 0 ? 0 : -1;
	}
	first = data[0];
	if (first < item) {
		return -1;
	}
	if (first > item) {
		return 1;
	}
	return length == 1 ? 0 : 1;
}
int datastring::compare(const char *item)
{
	char ch2;
	char ch1;
	int start = 0;
	if (item == nullptr) {
		return length > 0 ? 1 : 0;
	}
	if ((data == nullptr) || (length == 0)) {		
		return item[0] == 0 ? 0 : -1;
	}
	while(true) {
		ch2=*(item++);
		if (ch2 == 0) {
			break;
		}
		if (start >= length) {
			return -1;
		}
		ch1 = data[start];
		if (ch1 > ch2) {
			return 1;
		}
		if (ch1 < ch2) {
			return -1;
		}		
		start++;
	}
	return start == length ? 0 : 1;
}
int datastring::compare(datastring &item)
{
	int cmp;
	int cmp_length = length < item.length ? length : item.length;
	if (cmp_length == 0) {
		cmp = 0;
	} else {
		cmp = memcmp(data,item.data,cmp_length);
	}
	if (cmp == 0) {
		if (length > item.length) {
			cmp = 1;
		} else if (length < item.length) {
			cmp = -1;
		}
	}
	return cmp;
}
void datastring::remove_bytes_from_front(int number_of_bytes)
{
	// removes some bytes from the front of the string.
	if (number_of_bytes > length) {
		number_of_bytes = length;
	}
	data += number_of_bytes;
	length -= number_of_bytes;
	return;
}
void datastring::print()
{
	int loop;
	for(loop=0;loop<length;loop++) {
		printf("%c",data[loop]);
	}
	return;
}
void datastring::println()
{
	print();
	printf("\n");
	return;
}
void datastring::null_terminate()
{
	// Make sure this string is null terminated.
	if (data != nullptr) {
		data[length] = 0;
	}
	return;
}

void datastring::sha_256(datastring &output)
{
	const char *hex = "0123456789abcdef";
	char *output_data;
	int loop;
	sha256 hash;
	uint8_t input;
	
	if (output.length == SHA256_BLOCK_SIZE << 1) {
		output_data = output.data;		
		hash.sha256_init();
		hash.sha256_update((uint8_t *)data,length);
		hash.sha256_final((uint8_t *)output_data);
		// convert the output into hex.
		for(loop = SHA256_BLOCK_SIZE-1;loop>=0;loop--) {
			input = output_data[loop];		
			output_data[loop << 1] = hex[input >> 4];
			output_data[(loop << 1) + 1] = hex[input & 15];
		}
	}
	return;
}
