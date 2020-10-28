#include "chatcommand_readsnap.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "stringbuilder.h"
#include "message.h"
#include "Debug.h"
#include "chatclient.h"
#include <time.h>
#include <inttypes.h>

// read_snap(messageid,snapid)
// Call this after you read a snap.
// resends a snap message to both users of the snap or an error message.
bool chatcommand_readsnap::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'r') 
		|| (received_message->actual_message.substr(0,9)!="readsnap(")) {
		return false;
	}	
	
	Debug debug(__FILE__,__func__,__LINE__);
	message *new_message;
	datastring method_parameters;
	parameters parameters_parsed;
	bool parameter_success = true;
	chatroom *chat_room;
	int64_t messageid;
	int64_t snapid;
	snap *current_snap;
	datastring error_message;
	time_t datereceived;
	int64_t userid;
	bool im_from_user;
	bool im_to_user;
	time_t now;
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(9,received_message->actual_message.length-10);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);	
	
	debug = __LINE__;
	if (parameter_success) {
		// Give the parameters nice names.
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		snapid = parameters_parsed.long_parameters[1];
				
		if (client->logged_in_user == nullptr) {
			debug = __LINE__;
			error_message = "You need to be logged in to readsnap.";
			error(client,error_message,messageid);
		} else {			
			// Delete old snaps.
			debug = __LINE__;
			time(&now);
			snap::delete_old_snaps(the_websocket->snaps,now);
			// Find the snap.
			debug = __LINE__;
			current_snap = snap::findsnap(the_websocket->snaps,snapid);
			
			if (current_snap == nullptr) {
				// Chat room wasn't found.
				debug = __LINE__;
				error_message = "That snap wasn't found.";
				error(client,error_message,messageid);
			} else {
				debug = __LINE__;
				userid = client->logged_in_user->userid;
				im_from_user = (current_snap->from_userid == userid);
				im_to_user = (current_snap->to_userid == userid);
				if ((!im_from_user) && (!im_to_user)) {
					debug = __LINE__;
					error_message = "That is not your snap.";
					error(client,error_message,messageid);
				} else {
					time(&datereceived);
					current_snap->set_datereceived(datereceived);
					debug = __LINE__;
					client->add_message(current_snap->snap_message);
					client->callback_on_writable();
					
					if (im_from_user) {
						chatclient::send_message_to_user(&the_websocket->chatclients,current_snap->snap_message,current_snap->to_userid,true);
					}
					if (im_to_user) {
						chatclient::send_message_to_user(&the_websocket->chatclients,current_snap->snap_message,current_snap->from_userid,true);
					}
				}
			}
		}
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}


