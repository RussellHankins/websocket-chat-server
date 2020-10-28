#ifndef __CHATCOMMAND_READSNAP_H
#define __CHATCOMMAND_READSNAP_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"
#include "snap.h"

// Sends a snap.
class chatcommand_readsnap:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
