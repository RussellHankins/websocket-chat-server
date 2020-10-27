#ifndef __CHATCOMMAND_GETSNAPS_H
#define __CHATCOMMAND_GETSNAPS_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Sends a snap.
class chatcommand_getsnaps:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
