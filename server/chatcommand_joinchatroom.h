#ifndef __CHATCOMMAND_JOINCHATROOM_H
#define __CHATCOMMAND_JOINCHATROOM_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Joins a chatroom.
// joinchatroom(messageid,chatroomid,leave_other_chatrooms,answerhashed,server_password)
class chatcommand_joinchatroom:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
