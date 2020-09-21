#include "chatcommand_adduser.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"

// adduser(messageid,userid,username,password,server_password)
// Creates a new chatroom. Joins this user to the chatroom.
// Sends a message to all users that a chatroom was created.

bool chatcommand_adduser::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'a') 
		|| (received_message->actual_message.substr(0,8)!="adduser(")) {
		return false;
	}
	
	user *new_user;
	datastring method_parameters;
	parameters parameters_parsed;
	datastring error_message;
	bool parameter_success = true;
	bool secure;	
	int64_t messageid;	
	
	method_parameters = received_message->actual_message.substr(8,received_message->actual_message.length-9);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {
		messageid = parameters_parsed.long_parameters[0];
		secure = (parameters_parsed.string_parameters[4] == *(the_websocket->server_password));
		
		if (!secure) {
			incorrect_system_password(client,messageid);			
		} else {
			if (user::find(&the_websocket->users,parameters_parsed.string_parameters[1])!=nullptr) {
				error_message = "That userid is already in use.";
				error(client,error_message,messageid);
			} else {
				if (user::find(&the_websocket->users,parameters_parsed.string_parameters[2])!=nullptr) {
					error_message = "That user name is already in use.";
					error(client,error_message,messageid);
				} else {
					// Add the user.
					new_user = new user();
					new_user->userid = parameters_parsed.long_parameters[1];
					new_user->username = new datablock(parameters_parsed.string_parameters[2]);
					// Save the hash of the password. 
					// Ideally the password was already hashed before it was sent.
					// So, this is the hash of the hash of the password.
					new_user->password = new datablock(parameters_parsed.string_parameters[3]);
					the_websocket->users.add(new_user);
					
					success_message(client,messageid);
				}
			}
		}		
	} else {
		parameters_not_correct(client);
	}
	return true;
}
