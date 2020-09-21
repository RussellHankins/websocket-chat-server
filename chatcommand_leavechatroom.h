
#ifndef __CHATCOMMAND_LEAVECHATROOM_H
#define __CHATCOMMAND_LEAVECHATROOM_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// leave a chatroom.
// leavechatroom(messageid,chatroomid)
class chatcommand_leavechatroom:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif

