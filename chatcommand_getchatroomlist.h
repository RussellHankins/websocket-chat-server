#ifndef __CHATCOMMAND_GETCHATROOMLIST_H
#define __CHATCOMMAND_GETCHATROOMLIST_H
#include "chatcommand.h"
class chatcommand_getchatroomlist:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
