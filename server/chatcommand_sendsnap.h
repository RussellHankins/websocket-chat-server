#ifndef __CHATCOMMAND_SENDSNAP_H
#define __CHATCOMMAND_SENDSNAP_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// Sends a snap.
class chatcommand_sendsnap:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
	// Look at this user and send all the snaps to this user if they're logged on.
	static void send_snaps_to_user(int64_t userid);
	static void send_snaps_to_user(chatclient *client);
};
#endif
