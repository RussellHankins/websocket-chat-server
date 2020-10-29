#include "chatcommand_sendtoall.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "stringbuilder.h"

// sendtoall(server_password,messageid,message)
// Returns success(messageid) or error(message,messageid);
// sends privatemessage(chatclientid,userid,username,message) to all users.
// If user isn't logged in, userid = 0 and username is blank.

bool chatcommand_sendtoall::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 's') 
		|| (received_message->actual_message.substr(0,10)!="sendtoall(")) {
		return false;
	}	
	
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	stringbuilder output_message;
		
	method_parameters = received_message->actual_message.substr(10,received_message->actual_message.length-11);
	method_parameters.null_terminate(); // Used to comply with snprintf.
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {
		if (parameters_parsed.string_parameters[0] != *(the_websocket->server_password)) {
			// Incorrect server password.
			incorrect_system_password(client,parameters_parsed.long_parameters[1]);
		} else {
			// Build the output message.
			output_message += "privatemessage(";
			output_message.addparameter(client->chatclientid);
			if (client->logged_in_user == nullptr) {
				output_message += "0,0,";
			} else {
				output_message.addparameter(client->logged_in_user->userid);
				output_message.addparameter(*client->logged_in_user->username);
			}
			output_message.addparameter(parameters_parsed.string_parameters[2]);						
			output_message += ")";
			new_message = new message();
			*new_message = output_message;
			// send the output message to all clients.
			chatclient::send_message_to_clients(&(the_websocket->chatclients),new_message);
			message::dereference(&new_message);	
			// Send a success message be sent back to the client.
			success_message(client,parameters_parsed.long_parameters[1]);
		}
	} else {
		parameters_not_correct(client);
	}
	return true;
}
