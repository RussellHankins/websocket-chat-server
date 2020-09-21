#ifndef __CHATCOMMAND_SEND_H
#define __CHATCOMMAND_SEND_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Sends a message to all users in a chatroom.
class chatcommand_send:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
