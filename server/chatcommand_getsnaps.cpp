#include "chatcommand_getsnaps.h"
#include "chatcommand_sendsnap.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "biglist.h"
#include "Debug.h"
#include <stdio.h>
#include <time.h>

// getsnaps(messageid)
// After calling this, all of your snaps will start being sent to you.
// Returns a success message or an error message if you're not logged on.
bool chatcommand_getsnaps::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'g') 
		|| (received_message->actual_message.substr(0,9)!="getsnaps(")) {
		return false;
	}
	Debug debug(__FILE__,__func__,__LINE__);
	
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	datastring error_message;
	int64_t messageid;
	time_t now;
		
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(9,received_message->actual_message.length-10);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	debug = __LINE__;
	if (parameter_success) {
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		
		if (client->logged_in_user == nullptr) {
			error_message = "You need to be logged in to use getsnaps.";
			error(client,error_message,messageid);
		} else {
			client->latest_snapid = 0;
			success_message(client,messageid);
			// Delete any old snaps.
			time(&now);
			snap::delete_old_snaps(the_websocket->snaps,now);
			// Send this user all their snaps.
			chatcommand_sendsnap::send_snaps_to_user(client);
		}
	} else {
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}
