#ifndef __CHATCOMMAND_LOGIN_H
#define __CHATCOMMAND_LOGIN_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"
#include "user.h"

class chatcommand_login:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
	static void login(int64_t messageid,chatclient *client,user *logged_in_user,datastring &password,bool fast_queue);
	static void remotelogin(int64_t messageid,chatclient *client,datastring &username,datastring &password);
	private:
	static datablock *build_login_url(datastring &username,datastring &password);
	static void tell_all_chatrooms_this_user_logged_in
		(int64_t messageid,biglist<chatroom *> *chatrooms,
		int64_t chatclientid,user *logged_in_user,bool fast_queue);

};
#endif
