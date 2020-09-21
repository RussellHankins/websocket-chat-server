#ifndef __CHATCOMMAND_SENDTO_H
#define __CHATCOMMAND_SENDTO_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// sendto(messageid,chatclientid,userid,message)
// Send a message to a specific user.
class chatcommand_sendto:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
