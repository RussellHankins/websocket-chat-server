#ifndef __DATASTRING_H
#define __DATASTRING_H
// A read-only string of data.
class datastring
{
	public:
	char *data;
	int length;
	bool null_terminated; // Is this string null terminated?
	void remove_bytes_from_front(int number_of_bytes);	// removes some bytes from the front of the string.
	datastring substr(int start);
	datastring substr(int start,int len);
	void print();
	void println();
	void null_terminate(); // Make sure this string is null terminated.
	void sha_256(datastring &output); // It assumes output.length == SHA256_BLOCK_SIZE * 2
	int compare(const char *item);
	int compare(datastring &item);
	int compare(char item);
	datastring &operator=(const char *item);
	bool operator==(datastring &item) { return compare(item)==0; };
	bool operator!=(datastring &item) { return compare(item)!=0; };
	bool operator>(datastring &item) { return compare(item)>0; };
	bool operator<(datastring &item) { return compare(item)<0; };
	bool operator>=(datastring &item) { return compare(item)>=0; };
	bool operator<=(datastring &item) { return compare(item)<=0; };
	
	bool operator==(const char *item) { return compare(item)==0; };
	bool operator!=(const char *item) { return compare(item)!=0; };
	bool operator>(const char *item) { return compare(item)>0; };
	bool operator<(const char *item) { return compare(item)<0; };
	bool operator>=(const char *item) { return compare(item)>=0; };
	bool operator<=(const char *item) { return compare(item)<=0; };
	
	bool operator==(char item) { return compare(item)==0; };
	bool operator!=(char item) { return compare(item)!=0; };
	bool operator>(char item) { return compare(item)>0; };
	bool operator<(char item) { return compare(item)<0; };
	bool operator>=(char item) { return compare(item)>=0; };
	bool operator<=(char item) { return compare(item)<=0; };
};
#endif
