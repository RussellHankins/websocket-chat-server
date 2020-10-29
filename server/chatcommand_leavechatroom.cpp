#include "chatcommand_leavechatroom.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "chatcommand_logout.h"
#include "Debug.h"


// leaves a chatroom.
// Sends a message to all users in the chatroom that a user left.

bool chatcommand_leavechatroom::processmessage(char first_letter,message *received_message,chatclient *client)
{	
	if ((first_letter != 'l') 
		|| (received_message->actual_message.substr(0,14)!="leavechatroom(")) {
		return false;
	}
	Debug debug(__FILE__,__func__,__LINE__);
		
	chatroom *chatroom_to_leave;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	datastring error_message;
	int64_t messageid;
	int64_t chatroomid;
	
	debug = __LINE__;	
	method_parameters = received_message->actual_message.substr(14,received_message->actual_message.length-15);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	debug = __LINE__;
	if (parameter_success) {	
		// Give the parameters nice names.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
				
		// Find this chatroom.
		debug = __LINE__;
		chatroom_to_leave = chatroom::find(&the_websocket->chatrooms,chatroomid);
		if (chatroom_to_leave == nullptr) {
			debug = __LINE__;
			error_message = "That chatroom wasn't found.";
			error(client,error_message,messageid);
			return true;
		}
		debug = __LINE__;
		// Is this client currently in the room?
		if (!chatroom_to_leave->clients.find(client)) {
			debug = __LINE__;
			error_message = "You're not in this chatroom.";
			error(client,error_message,messageid);
			return true;
		}
		debug = __LINE__;
		// leave chat room.
		debug = __LINE__;
		chatroom_to_leave->remove_client(client);
		debug = __LINE__;
		client->remove_chatroom(chatroom_to_leave);				
		debug = __LINE__;
		
		if (chatroom_to_leave->number_of_clients > 0) {
			// Send a message to all the users in the chatroom that a person left.
			debug = __LINE__;
			new_message = userleftchatroom(messageid,chatroomid,client->chatclientid, client->logged_in_user,chatroom_to_leave->number_of_clients);
			debug = __LINE__;
			chatroom_to_leave->send_message_to_clients(new_message);
			// Send the same message back to the client that sent this request.
			client->push_message(&new_message);
		} else {
			// Delete this chatroom if it's empty and delete_if_unused.
			if (chatroom_to_leave->delete_if_unused) {
				debug = __LINE__;
				the_websocket->chatrooms.remove(chatroom_to_leave);
				debug = __LINE__;
				delete chatroom_to_leave;
				chatroom_to_leave = nullptr;
				// Tell all users the chatroom was deleted.
				debug = __LINE__;
				new_message = chatroomwasdeleted(messageid,chatroomid);
				debug = __LINE__;
				chatclient::send_message_to_clients(&(the_websocket->chatclients),new_message,true);
				message::dereference(&new_message);
			}
		}
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
