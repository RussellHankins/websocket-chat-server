#ifndef __READURL_H
#define __READURL_H
#include "stringbuilder.h"
#include "datablock.h"

class readurl
{
	public:
	static stringbuilder *read_url(const char *url,const char **error_message);
	private:
	static size_t write_callback(char *buf, size_t size, size_t nmemb, void *up);		
};
#endif
