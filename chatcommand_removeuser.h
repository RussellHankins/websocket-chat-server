#ifndef __CHATCOMMAND_REMOVEUSER_H
#define __CHATCOMMAND_REMOVEUSER_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Removes a user.
class chatcommand_removeuser:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
