#include "chatcommand_addchatroom.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"

// addchatroom(messageid,room_name,question,answer,answer_hashed,server_password)
// Creates a new chatroom. Joins this user to the chatroom.
// Sends a message to all users that a chatroom was created.

bool chatcommand_addchatroom::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'a') 
		|| (received_message->actual_message.substr(0,12)!="addchatroom(")) {
		return false;
	}
	
	chatroom *new_chatroom;
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	bool secure;
		
	method_parameters = received_message->actual_message.substr(12,received_message->actual_message.length-13);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);		
	if (parameter_success) {
		secure = (parameters_parsed.string_parameters[5] == *(the_websocket->server_password));
		
		// Create new_chatroom.
		new_chatroom = new chatroom();
		new_chatroom->chatroomid = the_websocket->next_chatroomid++;
		new_chatroom->name = new datablock(parameters_parsed.string_parameters[1]);
		new_chatroom->question = new datablock(parameters_parsed.string_parameters[2]);
		new_chatroom->answer = new datablock(parameters_parsed.string_parameters[3]);
		new_chatroom->answer_hashed = new datablock(parameters_parsed.string_parameters[4]);
		new_chatroom->everyone_can_join = !secure;
		new_chatroom->delete_if_unused = !secure;
			
		
		// This should have some thread locking of the chatrooms list in the future.
		// Currently, it doesn't need locking because there's only one work thread.
		new_chatroom->add_client(client); // Add this user to the chat room.
		client->add_chatroom(new_chatroom);
		
		if (client->logged_in_user != nullptr) {
			// Save that this user can access this chat room.
			client->logged_in_user->chatrooms_allowed.add(new_chatroom);
		}
		the_websocket->chatrooms.add(new_chatroom);
		
		// Send a message to the client that the chatroom is created.
		//chatroomcreated(messageid,chatroomid,room_name,info,number_of_clients)
		new_message = chatroomcreated(parameters_parsed.long_parameters[0],new_chatroom->chatroomid,parameters_parsed.string_parameters[1],new_chatroom->info(true),new_chatroom->number_of_clients);
		client->add_message(new_message);
		message::dereference(&new_message);
		client->callback_on_writable();
		
		// Send a message to all clients that the clatroom is created.
		// The parameter to info() is false instead of true.
		new_message = chatroomcreated(parameters_parsed.long_parameters[0],new_chatroom->chatroomid,parameters_parsed.string_parameters[1],new_chatroom->info(false),new_chatroom->number_of_clients);
		biglist_iterator<chatclient *>loop(&(the_websocket->chatclients));
		while (!loop.eof()) {
			if (loop.item != client) {
				loop.item->add_message(new_message);
			}
			loop.movenext();
		}
		message::dereference(&new_message);
		loop.movefirst();
		while (!loop.eof()) {
			if (loop.item != client) {
				loop.item->callback_on_writable();
			}
			loop.movenext();
		}
		
	} else {
		parameters_not_correct(client);
	}
	return true;
}
