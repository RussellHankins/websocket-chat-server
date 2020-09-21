#include "user.h"
#include "chatroom.h"
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
	chatrooms_allowed.clear(true);
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
	biglist_iterator<user *> loop(user_list);
	while (!loop.eof()) {
		if (loop.item->userid == user_id) {
			return loop.item;
		}
		loop.movenext();
	}	
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
