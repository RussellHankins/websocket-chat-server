#include "chatcommand_getusers.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "biglist.h"
#include "Debug.h"
#include <stdio.h>

// getusers(messageid,status,server_password)
// Gets the list of users in ram. Server_password is required.
// If status == 1, only return users that are logged in.
// If status == 2, only return users that are logged off.
// If status == 3, return both users logged in and users logged off.
		
// Outputs a message like users(messageid,[userid,username,status])
// status is 1 for logged in, 2 for logged out.

bool chatcommand_getusers::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'g') 
		|| (received_message->actual_message.substr(0,9)!="getusers(")) {
		return false;
	}
	Debug debug(__FILE__,__func__,__LINE__);
	
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	stringbuilder output_message;	
	int64_t messageid;
	int64_t input_status;
	int64_t output_status;
	datastring error_message;
	user *this_user;
	user *logged_in_user;
	bool found_user_range;
	bool secure;
	int64_t userid;
	int64_t smallest_userid;
	int64_t largest_userid;
	const int LOGGED_IN = 1;
	const int LOGGED_OUT = 2;
	const int RETURN_LOGGED_IN = 1;
	const int RETURN_LOGGED_OUT = 2;
	const int RETURN_ALL = 3;
		
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(9,received_message->actual_message.length-10);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	debug = __LINE__;
	if (parameter_success) {
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		input_status = parameters_parsed.long_parameters[1];
		secure = (parameters_parsed.string_parameters[2] == *(the_websocket->server_password));
		
		if (!secure) {
			incorrect_system_password(client,messageid);
		} else {
			output_message += "users(";
			output_message.addparameter(messageid);			
			if ((input_status < RETURN_LOGGED_IN) || (input_status > RETURN_ALL)) {
				error_message = "Invalid status code.";
				error(client,error_message,messageid);
				return true;
			}
			
			debug = __LINE__;
			biglist_iterator<chatclient *>client_loop(&the_websocket->chatclients);
			debug = __LINE__;
			biglist_iterator<user *>user_loop(&the_websocket->users);
			debug = __LINE__;
			found_user_range = false;
			smallest_userid = 9223372036854775807L; // 2^63-1
			smallest_userid = (smallest_userid - 1) + smallest_userid;
			largest_userid = 0;			
			debug = __LINE__;
			// Loop through all the users.
			while (!user_loop.eof()) {
				debug = __LINE__;
				this_user = user_loop.item;
				userid = this_user->userid;
				// Figure out if this user is logged in.
				output_status = LOGGED_OUT;
				if ((!found_user_range) || ((userid >= smallest_userid) && (userid <= largest_userid))) {
					client_loop.movefirst();
					while (!client_loop.eof()) {
						logged_in_user = client_loop.item->logged_in_user;
						if ((logged_in_user != nullptr) 
						&& (logged_in_user->userid == userid)) {
							output_status = LOGGED_IN;
							break;
						}
						client_loop.movenext();
					}
				}
				// Figure out if this user should be returned.
				if ((input_status == RETURN_ALL) 
				|| ((input_status == RETURN_LOGGED_IN) 
					&& (output_status == LOGGED_IN))
				|| ((input_status == RETURN_LOGGED_OUT) 
					&& (output_status == LOGGED_OUT))) {
					// Add this user to the output list.		
					output_message.addparameter(this_user->userid);
					output_message.addparameter(*this_user->username);
					output_message.addparameter(output_status);
				}				
				user_loop.movenext();			
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
		}
	} else {
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
