#include "chatcommand_logout.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "biglist.h"

// logout(messageid,disconnect)
// Logs out this user.
// Sends a success or error message.
// Sends a message to all users in all chatrooms this person is in that they're out.
bool chatcommand_logout::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'l') 
		|| (received_message->actual_message.substr(0,7)!="logout(")) {
		return false;
	}
	
	datastring method_parameters;
	parameters parameters_parsed;
	datastring error_message;
	user *logged_in_user;
	bool parameter_success = true;	
	bool disconnect;	
	
	method_parameters = received_message->actual_message.substr(7,received_message->actual_message.length-8);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	if (parameter_success) {
		disconnect = (parameters_parsed.long_parameters[1] != 0);
		if (!(disconnect) && (client->logged_in_user == nullptr)) {
			error_message = "You are already logged out.";
			error(client,error_message,parameters_parsed.long_parameters[0]);
		} else {
			exit_all_chatrooms(parameters_parsed.long_parameters[0],client, true);
			if (disconnect) {			
				// disconnect.
				client->disconnect();
			} else {
				// Send a success message.
				client->logged_in_user = nullptr;
				success_message(client,parameters_parsed.long_parameters[0]);
			}
		}
	} else {
		parameters_not_correct(client);
	}
	return true;
}

void chatcommand_logout::exit_all_chatrooms(int64_t messageid,chatclient *client, bool send_message)
{
	message *new_message;
	chatroom *room;
	if (client->chatrooms != nullptr) {
		biglist_iterator<chatroom *>chatroom_loop(client->chatrooms);
		while (!chatroom_loop.eof()) {
			room = chatroom_loop.item;
			if (send_message) {
				new_message = userleftchatroom(messageid,room->chatroomid,client->chatclientid,client->logged_in_user,room->number_of_clients);
				chatroom_loop.item->send_message_to_clients(new_message);
				message::dereference(&new_message);
			}
			room->remove_client(client);
			
			if ((room->delete_if_unused) && (room->number_of_clients == 0)) {
				// Delete this chatroom.
				if (send_message) {
					new_message = chatroomwasdeleted(messageid,room->chatroomid);
					chatclient::send_message_to_clients(&(the_websocket->chatclients),new_message,true);
					message::dereference(&new_message);
				}
				
				the_websocket->chatrooms.remove(room);
				delete room;
				room = nullptr;
			}
			
			chatroom_loop.row->used = false; // Remove this item from the list.
			chatroom_loop.movenext();			
		}
	}
	return;
}
void chatcommand_logout::exit_chatroom(
	int64_t messageid,chatclient *client,chatroom *room
	,bool send_message, bool remove_from_list)
{
	message *new_message;
	if (send_message) {
		new_message = userleftchatroom(messageid,room->chatroomid
		,client->chatclientid,client->logged_in_user,room->number_of_clients);
		room->send_message_to_clients(new_message);
		message::dereference(&new_message);
	}
	if (remove_from_list) {
		room->remove_client(client);	
		if ((room->delete_if_unused) && (room->number_of_clients == 0)) {
			// Delete this chatroom.
			new_message = chatroomwasdeleted(messageid,room->chatroomid);
			chatclient::send_message_to_clients(&(the_websocket->chatclients),new_message,true);
			message::dereference(&new_message);
			
			the_websocket->chatrooms.remove(room);
			delete room;
			room = nullptr;
		}
	}
	return;
}
