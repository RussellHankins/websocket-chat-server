#ifndef __CHATCOMMAND_ADDUSER_H
#define __CHATCOMMAND_ADDUSER_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Adds a new user.
class chatcommand_adduser:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
