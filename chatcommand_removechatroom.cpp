#include "chatcommand_removechatroom.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "Debug.h"

// removechatroom(messageid,chatroomid,server_password)
// Removes a chatroom. Requires server password.
// Sends a chatroomwasdeleted message to you and everybody in the chatroom.

bool chatcommand_removechatroom::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'r') 
		|| (received_message->actual_message.substr(0,15)!="removechatroom(")) {
		return false;
	}
	
	Debug debug(__FILE__,__func__,__LINE__);	
	chatroom *this_chatroom;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;	
	bool parameter_success = true;
	bool secure;
	datastring error_message;
	int64_t messageid;
	int64_t chatroomid;
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(15,received_message->actual_message.length-16);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {		
		// Copy parameters into permanent containers.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		chatroomid = parameters_parsed.long_parameters[1];
		secure = (parameters_parsed.string_parameters[2] == *(the_websocket->server_password));
		
		if (!secure) {
			debug = __LINE__;
			error_message = "Incorrect server password.";
			error(client,error_message,messageid);
			return true;
		}
		
		// Find this chatroom.
		this_chatroom = nullptr;
		debug = __LINE__;
		biglist_iterator<chatroom *>chatroom_loop(&the_websocket->chatrooms);
		while (!chatroom_loop.eof()) {
			debug = __LINE__;
			if (chatroom_loop.item->chatroomid == chatroomid) {
				this_chatroom = chatroom_loop.item;
				chatroom_loop.row->used = false; // Remove this chatroom from the list of chatrooms.
				break;
			}
			debug = __LINE__;
			chatroom_loop.movenext();
		}
		debug = __LINE__;
		if (this_chatroom == nullptr) {
			debug = __LINE__;
			error_message = "That chatroom wasn't found.";
			error(client,error_message,messageid);
			return true;
		}
		
		// Finish removing this chatroom from all clients in the chatroom.
		debug = __LINE__;
		biglist_iterator<chatclient *>chatclient_loop(&this_chatroom->clients);
		while (!chatclient_loop.eof()) {
			chatclient_loop.item->remove_chatroom(this_chatroom);
			chatclient_loop.movenext();
		}		
		delete this_chatroom;
		this_chatroom = nullptr;
				
		// Send a message to all users in this chatroom was deleted.
		debug = __LINE__;
		new_message = chatroomwasdeleted(messageid,chatroomid);
		chatclient::send_message_to_clients(&(the_websocket->chatclients),new_message);
		message::dereference(&new_message);
		
		debug = __LINE__;				
	} else {
		parameters_not_correct(client);
	}
	return true;
}
