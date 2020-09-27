#ifndef __CHATCOMMAND_LOGIN_H
#define __CHATCOMMAND_LOGIN_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

class chatcommand_login:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
