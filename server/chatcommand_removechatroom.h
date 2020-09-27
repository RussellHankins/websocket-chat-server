#ifndef __CHATCOMMAND_REMOVECHATROOM_H
#define __CHATCOMMAND_REMOVECHATROOM_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Removes a new chatroom.
class chatcommand_removechatroom:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
