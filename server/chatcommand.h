#ifndef __CHATCOMMAND_H
#define __CHATCOMMAND_H
#include "message.h"
#include "chatclient.h"
#include "datastring.h"
#include "user.h"

// This is the base class for all chat commands.
class chatcommand
{
	public:
	//virtual bool processmessage(message *chatmessage,chatclient *client) = 0; // Returns true if the message was processed.
	static void incorrect_system_password(chatclient *client,int64_t messageid);
	static void not_implemented(chatclient *client);
	static void parameters_not_correct(chatclient *client);	
	static void message_not_understood(chatclient *client);
	static void success_message(chatclient *client,int64_t messageid);
	static void error(chatclient *client,datastring error_message,int64_t messageid);
	static void error(chatclient *client,datastring error_message);
	// These methods create messages that the client will understand.
	// They don't actually send the message. The calling method does that.
	static message *userjoinedchatroom(int64_t messageid,int64_t chatroomid,int64_t chatclientid, user *new_user,int number_of_clients);
	static message *userleftchatroom(int64_t messageid,int64_t chatroomid,int64_t chatclientid,user *old_user,int number_of_clients);	
	static message *chatroomwasdeleted(int64_t messageid,int64_t chatroomid);
	static message *chatroomdetails(int64_t messageid,int64_t chatroomid,datastring *room_name,int info,int number_of_clients,datastring *question);
	static message *chatroomcreated(int64_t messageid,int64_t chatroomid,datastring room_name,int info,int number_of_clients);
	static message *yourchatclientid(int64_t chatclientid);
	static message *chat(int64_t messageid,int64_t chatroomid,int64_t chatclientid,int64_t userid,datastring message_to_send);
	static message *msg(int64_t messageid,int64_t chatclientid,int64_t userid,datastring message_to_send);
};

#endif
