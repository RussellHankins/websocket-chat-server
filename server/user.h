#ifndef __USER_H
#define __USER_H
#include "datablock.h"
#include "biglist.h"
#include "file_tree.h"
#include <cstdint>

class chatroom; // Forward declaration https://stackoverflow.com/questions/8526819/c-header-files-including-each-other-mutually

class user
{
	public:
	int64_t userid;	
	datablock *username;
	datablock *password;	
	user();
	~user();
	void initialize();
	void clear();
	static user *find(biglist<user *>*user_list,datastring user_name);
	static user *find(biglist<user *>*user_list,int64_t user_id);
};
#endif
