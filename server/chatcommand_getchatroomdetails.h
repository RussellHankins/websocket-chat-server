#ifndef __CHATCOMMAND_GETCHATROOMDETAILS_H
#define __CHATCOMMAND_GETCHATROOMDETAILS_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Gets the details of a chatroom.
// getchatroomdetails(messageid,chatroomid)
class chatcommand_getchatroomdetails:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
