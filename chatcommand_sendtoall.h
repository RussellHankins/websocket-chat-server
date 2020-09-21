#ifndef __CHATCOMMAND_SENDTOALL_H
#define __CHATCOMMAND_SENDTOALL_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Sends a message to all users. Requires server password.
class chatcommand_sendtoall:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
