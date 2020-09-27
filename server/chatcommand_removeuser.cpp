#include "chatcommand_removeuser.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"

// removeuser(messageid,userid,server_password)
// Removes a user.

bool chatcommand_removeuser::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'r') 
		|| (received_message->actual_message.substr(0,11)!="removeuser(")) {
		return false;
	}
	
	user *this_user;
	datastring method_parameters;
	parameters parameters_parsed;
	datastring error_message;
	bool parameter_success = true;
	bool secure;
	int64_t messageid;
	int64_t userid;	
	
	method_parameters = received_message->actual_message.substr(11,received_message->actual_message.length-12);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {
		messageid = parameters_parsed.long_parameters[0];
		userid = parameters_parsed.long_parameters[1];
		secure = (parameters_parsed.string_parameters[2] == *(the_websocket->server_password));
		
		if (!secure) {
			incorrect_system_password(client,messageid);
		} else {
			// Look to see if this user is logged in.
			biglist_iterator<chatclient *>client_loop(&the_websocket->chatclients);
			while (!client_loop.eof()) {
				this_user = client_loop.item->logged_in_user;
				if ((this_user != nullptr) && (this_user->userid == userid)) {
					// this user was found. She is still logged in.
					break;
				}
				client_loop.movenext();
			}
			if (!client_loop.eof()) {
				error_message = "That user is still logged in.";
				error(client,error_message,messageid);
			} else {
				biglist_iterator<user *>user_loop(&the_websocket->users);
				while (!user_loop.eof()) {					
					if (user_loop.item->userid == userid) {
						// Delete this user from the user list.
						delete user_loop.row->item;
						user_loop.row->item = nullptr;
						user_loop.row->used = false;
						break;
					}
				}
				// Send a success message.
				success_message(client,messageid);
			}
		}		
	} else {
		parameters_not_correct(client);
	}
	return true;
}
