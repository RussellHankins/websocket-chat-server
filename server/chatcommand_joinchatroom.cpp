#include "chatcommand_joinchatroom.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "chatcommand_logout.h"
#include "Debug.h"


// joinchatroom(messageid,chatroomid,leave_other_chatrooms,answerhashed)
// joins a chatroom.
// Sends a message to all users in the chatroom that a user joined.

bool chatcommand_joinchatroom::processmessage(char first_letter,message *received_message,chatclient *client)
{	
	if ((first_letter != 'j') 
		|| (received_message->actual_message.substr(0,13)!="joinchatroom(")) {
		return false;
	}
	Debug debug(__FILE__,__func__,__LINE__);
		
	chatroom *chatroom_to_join;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	bool allowed;
	stringbuilder output_message;
	datastring error_message;
	int64_t messageid;
	int64_t chatroomid;
	int64_t leave_other_chatrooms;
	datastring answer_hashed;
	
	debug = __LINE__;	
	method_parameters = received_message->actual_message.substr(13,received_message->actual_message.length-14);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);

	debug = __LINE__;
	if (parameter_success) {	
		// Give the parameters nice names.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
		leave_other_chatrooms = parameters_parsed.long_parameters[2];
		answer_hashed = parameters_parsed.string_parameters[3];

		// Find this chatroom.
		debug = __LINE__;
		chatroom_to_join = chatroom::find(&the_websocket->chatrooms,chatroomid);
		if (chatroom_to_join == nullptr) {
			debug = __LINE__;
			error_message = "That chatroom wasn't found.";
			error(client,error_message,messageid);
			return true;
		}
		// Check security:
		debug = __LINE__;
		error_message = "answer_hashed=";
		if ((chatroom_to_join->answer_hashed != nullptr) 
		&& (chatroom_to_join->answer_hashed->length > 0)
		&& (*chatroom_to_join->answer_hashed != answer_hashed)) {
			error_message = "Sorry. That answer was wrong.";
			error(client,error_message,messageid);
			return true;
		}
		
		debug = __LINE__;
		// Is this client already in the room?
		if (chatroom_to_join->clients.find(client)) {
			debug = __LINE__;
			error_message = "You're already in this chatroom.";
			error(client,error_message,messageid);
			return true;
		}
		debug = __LINE__;
		if (leave_other_chatrooms != 0) {
			// Leave all other chatrooms.
			debug = __LINE__;
			chatcommand_logout::exit_all_chatrooms(messageid,client,true);
		}
		// Join chat room.
		debug = __LINE__;
		chatroom_to_join->add_client(client);
		client->add_chatroom(chatroom_to_join);
		// Send a message to all the users in the chatroom that a new person joined.
		debug = __LINE__;
		new_message = userjoinedchatroom(messageid,chatroom_to_join->chatroomid,client->chatclientid, client->logged_in_user,chatroom_to_join->number_of_clients);
		chatroom_to_join->send_message_to_clients(new_message);
		message::dereference(&new_message);
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
