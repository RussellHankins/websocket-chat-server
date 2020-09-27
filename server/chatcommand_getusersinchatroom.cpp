#include "chatcommand_getusersinchatroom.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "biglist.h"
#include "Debug.h"
#include <stdio.h>

// getusersinchatroom(messageid,chatroomid)
// Gets the list of users in a chatroom.

bool chatcommand_getusersinchatroom::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'g') 
		|| (received_message->actual_message.substr(0,19)!="getusersinchatroom(")) {
		return false;
	}
	Debug debug(__FILE__,__func__,__LINE__);
	
	chatroom *this_chatroom;
	chatclient *loop_client;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	stringbuilder output_message;	
	int64_t messageid;
	int64_t chatroomid;	
	datastring error_message;
	user *this_user;
	bool is_user_in_chatroom;
	int64_t chatclientid;
	int64_t userid;
		
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(19,received_message->actual_message.length-20);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	debug = __LINE__;
	if (parameter_success) {
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
		chatclientid = client->chatclientid;
		this_user = client->logged_in_user;
		userid = this_user == nullptr ? 0 : this_user->userid;
		
		// Find this chatroom.
		debug = __LINE__;
		this_chatroom = chatroom::find(client->chatrooms,chatroomid);
		if (this_chatroom == nullptr) {
			debug = __LINE__;
			error_message = "That chatroom wasn't found.";
			error(client,error_message,messageid);
			return true;
		}
		
		// Is this user in the chatroom?
		debug = __LINE__;
		is_user_in_chatroom = false;
		biglist_iterator<chatclient *>client_loop(&this_chatroom->clients);
		while (!client_loop.eof()) {
			if (client_loop.item->chatclientid == chatclientid) {
				is_user_in_chatroom = true;
				break;
			}
			if ((userid != 0) 
			&& (client_loop.item->logged_in_user != nullptr)
			&& (client_loop.item->logged_in_user->userid == userid)) {
				is_user_in_chatroom = true;
				break;
			}
			client_loop.movenext();
		}
		debug = __LINE__;
		if (!is_user_in_chatroom) {
			error_message = "You are not in that chatroom.";
			error(client,error_message,messageid);
			return true;
		}
		
		// Build the output message.
		// usersinchatroom(messageid,chatroomid,[chatclientid,userid,username])
		debug = __LINE__;
		output_message += "usersinchatroom(";
		output_message.addparameter(messageid);
		output_message.addparameter(chatroomid);
		
		debug = __LINE__;
		client_loop.movefirst();
		debug = __LINE__;
		while(!client_loop.eof()) {
			debug = __LINE__;
			loop_client = client_loop.item;
			output_message.addparameter(loop_client->chatclientid);
			debug = __LINE__;
			this_user = loop_client->logged_in_user;
			if (this_user == nullptr) {
				output_message += "0,0,";
			} else {
				output_message.addparameter(this_user->userid);
				output_message.addparameter(*this_user->username);
			}			
			client_loop.movenext();
		}
		debug = __LINE__;
		output_message += ")";
		new_message = new message();
		*new_message = output_message;
		debug = __LINE__;
		output_message.clear();
		// Send the message.
		client->push_message(&new_message);
		debug = __LINE__;
	} else {
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
