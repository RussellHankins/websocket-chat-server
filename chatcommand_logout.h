#ifndef __CHATCOMMAND_LOGOUT_H
#define __CHATCOMMAND_LOGOUT_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

class chatcommand_logout:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
	
	// Remove this user from every chatroom.
	// Send a message to everyone in every chatroom that 
	static void exit_all_chatrooms(int64_t messageid,chatclient *client, bool send_message);
	
	// Remove this user from this chatroom.
	// Send a message to everyone in the chatroom that this client logged out.
	static void exit_chatroom(int64_t messageid,chatclient *client,chatroom *room, bool send_message, bool remove_from_list);
};

#endif
