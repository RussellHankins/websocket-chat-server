#include "chatcommand_send.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "stringbuilder.h"
#include "message.h"
#include "Debug.h"
#include <inttypes.h>

// send(messageid,chatroomid,message)
// Send a chat message to everybody in the chatroom.
// Returns chat(messageid,chatroomid,chatclientid,userid,message) or error(message,messageid);
// If user isn't logged in, userid = 0.

bool chatcommand_send::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 's') 
		|| (received_message->actual_message.substr(0,5)!="send(")) {
		return false;
	}	
	
	Debug debug(__FILE__,__func__,__LINE__);
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	chatroom *chat_room;
	int64_t messageid;
	int64_t chatroomid;
	datastring message_to_send;
	int64_t userid;
	datastring error_message;
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(5,received_message->actual_message.length-6);
	method_parameters.null_terminate(); // Used to comply with snprintf.
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	
	debug = __LINE__;
	if (parameter_success) {
		// Give the parameters nice names.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
		message_to_send = parameters_parsed.string_parameters[2];
		userid = client->logged_in_user == nullptr ? 0 : client->logged_in_user->userid;
		
		// Find the chatroom.		
		debug = __LINE__;
		//received_message->actual_message.println();
		//printf("chatroomid=%" PRId64 "\n",chatroomid);		
		chat_room = chatroom::find(client->chatrooms,chatroomid);
		if (chat_room == nullptr) {
			// Chat room wasn't found.
			debug = __LINE__;
			error_message = "You're not in that chatroom.";
			error(client,error_message,messageid);
		} else {
			// Build the output message.
			debug = __LINE__;
			new_message = chat(messageid,chatroomid,client->chatclientid,userid,message_to_send);
			
			// send the output message.
			debug = __LINE__;
			chat_room->send_message_to_clients(new_message);
			message::dereference(&new_message);
		}
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
