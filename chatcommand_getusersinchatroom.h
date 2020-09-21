#ifndef __CHATCOMMAND_GETUSERSINCHATROOM_H
#define __CHATCOMMAND_GETUSERSINCHATROOM_H
#include "chatcommand.h"
class chatcommand_getusersinchatroom:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
