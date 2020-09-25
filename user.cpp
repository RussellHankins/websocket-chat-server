#include "user.h"
#include "chatroom.h"
#include "Debug.h"
user::user()
{
	initialize();
}
user::~user()
{
	clear();
}
void user::clear()
{
	userid = 0;
	datablock::dereference(&username);
	datablock::dereference(&password);	
	return;
}
void user::initialize()
{
	username = nullptr;
	password = nullptr;
	userid = 0;
	return;
}
user *user::find(biglist<user *>*user_list,int64_t user_id)
{
	Debug debug(__FILE__,__func__,__LINE__);
	biglist_iterator<user *> loop(user_list);
	debug = __LINE__;
	while (!loop.eof()) {
		debug = __LINE__;
		if (loop.item->userid == user_id) {
			debug = __LINE__;
			return loop.item;
		}
		debug = __LINE__;
		loop.movenext();
	}
	debug = __LINE__;
	return nullptr;
}
user *user::find(biglist<user *>*user_list,datastring user_name)
{
	biglist_iterator<user *> loop(user_list);
	while (!loop.eof()) {
		if (*(loop.item->username) == user_name) {
			return loop.item;
		}
		loop.movenext();
	}	
	return nullptr;
}
