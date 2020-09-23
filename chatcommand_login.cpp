#include "chatcommand_login.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"

// login(messageid,username,password)
// Logs this connection in to the user.
// Sends a success or error message.

bool chatcommand_login::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'l') 
		|| (received_message->actual_message.substr(0,6)!="login(")) {
		return false;
	}
	
	message *new_message;
	user *new_user;
	datastring method_parameters;
	parameters parameters_parsed;
	datastring error_message;
	user *logged_in_user;
	bool parameter_success = true;
	stringbuilder output;
	
	method_parameters = received_message->actual_message.substr(6,received_message->actual_message.length-7);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {		
		logged_in_user = user::find(&the_websocket->users,parameters_parsed.string_parameters[1]);
		if ((logged_in_user == nullptr) 
		|| (*logged_in_user->password != parameters_parsed.string_parameters[2])) {
			error_message = "Incorrect login.";
			error(client,error_message,parameters_parsed.long_parameters[0]);
		} else {
			
			client->logged_in_user = logged_in_user;
			if (client->chatrooms != nullptr) {
				// Send a message to all chatrooms that this user logged in.				
				biglist_iterator<chatroom *>chatroom_loop(client->chatrooms);
				while (!chatroom_loop.eof()) {
					// Send a message to all clients that this user logged in.
					new_message = userjoinedchatroom(parameters_parsed.long_parameters[0],chatroom_loop.item->chatroomid,client->chatclientid,logged_in_user,chatroom_loop.item->number_of_clients);
					chatclient::send_message_to_clients(&chatroom_loop.item->clients,new_message);
					message::dereference(&new_message);
					chatroom_loop.movenext();
				}
			}
			success_message(client,parameters_parsed.long_parameters[0]);
		}
	} else {
		parameters_not_correct(client);
	}
	return true;
}
