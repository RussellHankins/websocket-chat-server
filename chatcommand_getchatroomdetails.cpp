#include "chatcommand_getchatroomdetails.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "Debug.h"

// getchatroomdetails(messageid,chatroomid)
bool chatcommand_getchatroomdetails::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'g') 
		|| (received_message->actual_message.substr(0,19)!="getchatroomdetails(")) {
		return false;
	}
	
	Debug debug(__FILE__,__func__,__LINE__);
	chatroom *this_chatroom;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	int64_t messageid;
	int64_t chatroomid;
	datastring error_message;
	int info;
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(19,received_message->actual_message.length-20);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	if (parameter_success) {		
		debug = __LINE__;
		// Get parameters.
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
		
		// Find this chatroom.
		debug = __LINE__;
		this_chatroom = chatroom::find(&the_websocket->chatrooms,chatroomid);
		if (this_chatroom == nullptr) {
			debug = __LINE__;
			error_message = "That chatroom wasn't found.";
			error(client,error_message,messageid);
			return true;
		}
		
		// Build the message.
		debug = __LINE__;
		info = this_chatroom->info(client);
		debug = __LINE__;
		new_message = chatroomdetails(messageid,chatroomid
		,this_chatroom->name,info,this_chatroom->number_of_clients,this_chatroom->question);
		
		// Send the message.
		debug = __LINE__;
		client->push_message(&new_message);
	} else {
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
