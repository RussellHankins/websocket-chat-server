#ifndef __CHATCOMMAND_GETUSERS_H
#define __CHATCOMMAND_GETUSERS_H
#include "chatcommand.h"
class chatcommand_getusers:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
