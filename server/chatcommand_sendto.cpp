#include "chatcommand_sendto.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "stringbuilder.h"
#include "message.h"
#include "Debug.h"
#include <inttypes.h>

// sendto(messageid,chatclientid,userid,message)
// Send a message to a specific user.
// Returns chat(messageid,chatroomid,chatclientid,userid,message) or error(message,messageid);
// If user isn't logged in, userid = 0.

bool chatcommand_sendto::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 's') 
		|| (received_message->actual_message.substr(0,7)!="sendto(")) {
		return false;
	}	
	
	Debug debug(__FILE__,__func__,__LINE__);
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	int64_t messageid;
	int64_t chatclientid;
	datastring message_to_send;
	int64_t userid;
	user *loop_user;
	datastring error_message;	
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(7,received_message->actual_message.length-8);
	method_parameters.null_terminate(); // Used to comply with snprintf.
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	
	debug = __LINE__;
	if (parameter_success) {
		// Give the parameters nice names.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatclientid = parameters_parsed.long_parameters[1];
		userid = parameters_parsed.long_parameters[2];		
		message_to_send = parameters_parsed.string_parameters[3];		
		
		// find the user. Look in chatclients.
		debug = __LINE__;
		biglist_iterator<chatclient *>loop(&the_websocket->chatclients);
		while (!loop.eof())
		{
			debug = __LINE__;
			if (userid != 0) {				
				loop_user = loop.item->logged_in_user;
				debug = __LINE__;
				if ((loop_user != nullptr) && (loop_user->userid == userid)) {
					break;
				}
			} else {
				debug = __LINE__;
				if (loop.item->chatclientid == chatclientid) {
					break;
				}
			}			
			loop.movenext();
		}
		debug = __LINE__;
		if (loop.eof()) {
			debug = __LINE__;
			error_message = "That person is not online.";
			error(client,error_message,messageid);
			return true;
		}
		debug = __LINE__;
		// Send the message to the user.
		new_message = msg(messageid,client->chatclientid,client->logged_in_user == nullptr ? 0 : client->logged_in_user->userid,message_to_send);
		loop.item->push_message(&new_message);
		// Send a success message to the client.
		success_message(client,messageid);
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
