#include "chatcommand_sendsnap.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "snap.h"
#include "biglist.h"
#include "Debug.h"

// sendsnap(messageid,userid,snap)
// Sends a snap. Snaps delete after 48 hours.
// The user doesn't have to be online to receive it.
// A snap_sent message or error message is returned.
bool chatcommand_sendsnap::processmessage(
char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 's') 
		|| (received_message->actual_message.substr(0,9)!="sendsnap(")) {
		return false;
	}
	
	Debug debug(__FILE__,__func__,__LINE__);
	user *new_user;
	datastring method_parameters;
	parameters parameters_parsed;
	datastring error_message;
	bool parameter_success = true;
	bool secure;	
	int64_t messageid;	
	int64_t userid;
	message *snapmessage;
	snap *new_snap;
	int64_t snapid;
	time_t old_time;
	time_t datesent;
	
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(9,received_message->actual_message.length-10);	
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);	
	
	debug = __LINE__;
	if (parameter_success) {
		messageid = parameters_parsed.long_parameters[0];
		userid = parameters_parsed.long_parameters[1];		
		
		if (client->logged_in_user == nullptr) {
			debug = __LINE__;
			error_message = "You must be logged in to send a snap.";
			error(client,error_message,messageid);
		} else {			
			// Delete old snaps that are more than two days old.
			debug = __LINE__;
			time(&datesent);
			old_time = datesent - snap::snap_age;			
			snap::delete_old_snaps(the_websocket->snaps,old_time);
			// Create a new snap message.
			snapid = the_websocket->next_snapid++;
			snapmessage = snap_message(snapid,userid,client->logged_in_user->userid,datesent,parameters_parsed.string_parameters[2]);
			debug = __LINE__;
			// Create a snap object.
			new_snap = new snap(snapid,snapmessage,client->logged_in_user->userid,userid,datesent);
			// Add the new snap object.
			debug = __LINE__;
			the_websocket->snaps.enqueue(new_snap);
			debug = __LINE__;
			message::dereference(&snapmessage);
			new_snap = nullptr;
			// Send a confirmation message to this user.
			debug = __LINE__;			
			snap_sent(client,messageid,snapid);
			// Send any and all snaps to the target user.
			send_snaps_to_user(userid);
		}
	} else {
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}

// Look at this user and send all the snaps to this user if they're logged on.
void chatcommand_sendsnap::send_snaps_to_user(int64_t userid)
{
	user *logged_in_user;
	chatclient *client;
	biglist_iterator<chatclient *>client_loop(&the_websocket->chatclients);
	while (!client_loop.eof()) {
		client = client_loop.item;
		logged_in_user = client->logged_in_user;
		if ((logged_in_user != nullptr) && (logged_in_user->userid == userid)) {
			send_snaps_to_user(client);
		}
		client_loop.movenext();
	}
	return;
}
// Look for all the snaps for this user and send them.
void chatcommand_sendsnap::send_snaps_to_user(chatclient *client)
{
	int64_t snapid;
	snap *item;
	user *logged_in_user;	
	int64_t userid;
	concurrent_queue<snap *> *snaps;
	bool added_message;
	
	added_message = false;	
	logged_in_user = client->logged_in_user;	
	if (logged_in_user != nullptr) 
	{
		snaps = &the_websocket->snaps;
		userid = logged_in_user->userid;
		snapid = client->latest_snapid;
		snaps->movefirst();
		while (snaps->current != nullptr) {
			item = snaps->current->item;
			if ((item->snapid >= snapid) 
			&& ((item->to_userid == userid) || (item->from_userid == userid))) {
				// Send this snap to the user.
				client->add_message(item->data);
				client->latest_snapid = item->snapid + 1;
				added_message = true;
			}
			snaps->movenext();
		}
	}
	if (added_message) {
		client->callback_on_writable();
	}
	return;
}
