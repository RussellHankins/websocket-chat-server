#ifndef __CHATCOMMAND_ADDCHATROOM_H
#define __CHATCOMMAND_ADDCHATROOM_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Adds a new chatroom.
class chatcommand_addchatroom:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
