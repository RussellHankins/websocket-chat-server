#include "chatcommand_getchatroomlist.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "biglist.h"
#include <stdio.h>

// getchatroomlist(messageid)
// Gets the list of chatrooms.

bool chatcommand_getchatroomlist::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'g') 
		|| (received_message->actual_message.substr(0,16)!="getchatroomlist(")) {
		return false;
	}	
	
	chatroom *new_chatroom;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	stringbuilder output_message;	
	bool show_chatroom;
	int64_t userid;
		
	method_parameters = received_message->actual_message.substr(16,received_message->actual_message.length-17);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	if (parameter_success) {
		userid = client->logged_in_user == nullptr ? 0 : client->logged_in_user->userid;
		// Build the output message.
		// This should have some thread locking of the chatrooms list in the future.
		// Currently, it doesn't need locking because there's only one work thread.
		//chatroomlist(messageid,number_of_chatrooms,[chatroomid,room_name])
		output_message += "chatroomlist(";
		output_message.addparameter(parameters_parsed.long_parameters[0]);
		biglist_iterator<chatroom *>loop(&the_websocket->chatrooms);
		while(!loop.eof()) {
			auto chatroom = loop.item;
			// Should this chat room be shown to this user?
			show_chatroom = false;
			if (chatroom->everyone_can_join) {
				show_chatroom = true;
			} else {
				if ((userid != 0) 
				&& (user::find(&chatroom->allowed_users,userid) != nullptr)) {
					show_chatroom = true;
				}
			}
			if (show_chatroom) {
				// Add this chat room to the list of chatrooms to show.
				output_message.addparameter(chatroom->chatroomid);
				output_message.addparameter(*chatroom->name);
				output_message.addparameter(chatroom->info(client));
				output_message.addparameter(chatroom->number_of_clients);
			}
			loop.movenext();
		}
		output_message += ")";
		new_message = new message();
		*new_message = output_message;
		output_message.clear();
		// Send the message.
		client->add_message(new_message);
		client->callback_on_writable();
		message::dereference(&new_message);
	} else {
		parameters_not_correct(client);
	}
	return true;
}
