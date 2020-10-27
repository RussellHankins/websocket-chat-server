#include "task.h"
#include "websocket.h"
#include "parameters.h"
#include <libwebsockets.h>
#include <stdio.h>
#include "message.h"
#include "Debug.h"
#include "chatcommand_sendtoall.h"
#include "chatcommand_addchatroom.h"
#include "chatcommand_getchatroomlist.h"
#include "chatcommand_adduser.h"
#include "chatcommand_login.h"
#include "chatcommand_logout.h"
#include "chatcommand_joinchatroom.h"
#include "chatcommand_send.h"
#include "chatcommand_leavechatroom.h"
#include "chatcommand_getusersinchatroom.h"
#include "chatcommand_removechatroom.h"
#include "chatcommand_sendto.h"
#include "chatcommand_removeuser.h"
#include "chatcommand_nop.h"
#include "chatcommand_getusers.h"
#include "chatcommand_getchatroomdetails.h"
#include "chatcommand_sendsnap.h"
#include "chatcommand_getsnaps.h"

task::task()
{
	started = true;
	finished = true; // Make it look like this task finished and can be reused.
	_client = nullptr;
	_message = nullptr;
}

void task::closeconnection(chatclient *client,bool run_async)
{
	Debug debug(__FILE__,__func__,__LINE__);
	_client = client;
	client->wsi = nullptr;
	debug = __LINE__;
	initialize_task(task_type::close_connection,run_async);
	debug = __LINE__;
	return;
}

void task::initialize_task(task_type tasktype,bool run_async)
{
	_task_type = tasktype;
	started = false;
	finished = false;	
	running_async = run_async;
	return;
}

void task::receivedmessage(chatclient *client,message *new_message,bool run_async)
{
	Debug debug(__FILE__,__func__,__LINE__);
	_client = client;
	_message = new_message->clone();
	debug = __LINE__;
	initialize_task(task_type::received_message,run_async);
	debug = __LINE__;
	return;	
}

void task::dotask()
{	
	started = true;
	Debug debug(__FILE__,__func__,__LINE__);
	switch (_task_type) {
		case task_type::close_connection:
		{
			debug = __LINE__;
			closeconnection();
			break;
		}
		case task_type::received_message:
		{
			debug = __LINE__;
			receivedmessage();
			break;
		}
	}
	debug = __LINE__;
	finished = true;
	// Clean up.
	_client = nullptr;
	message::dereference(&_message);
	return;
}

void task::closeconnection()
{
	Debug debug(__FILE__,__func__,__LINE__);
	message *new_message;
	// The user closed the connection.
	// Remove them from any chatrooms.
	if (_client->chatrooms != nullptr) {
		debug = __LINE__;
		biglist_iterator<chatroom *> chatroom_loop(_client->chatrooms);
		debug = __LINE__;
		while (!chatroom_loop.eof())
		{
			debug = __LINE__;
			// Remove this user from the chatroom.
			chatroom_loop.item->remove_client(_client);
			debug = __LINE__;
			//  Tell all the users in this chatroom that the user left.
			new_message = chatcommand::userleftchatroom(0,chatroom_loop.item->chatroomid,_client->chatclientid,_client->logged_in_user,chatroom_loop.item->number_of_clients);
			chatroom_loop.item->send_message_to_clients(new_message);
			message::dereference(&new_message);
			debug = __LINE__;
			
			// Is this chatroom empty? Should it be deleted?
			if ((chatroom_loop.item->delete_if_unused)
			&& (chatroom_loop.item->number_of_clients == 0)) {
				// Tell other clients that this chatroom is gone.
				debug = __LINE__;
				new_message = chatcommand::chatroomwasdeleted(0,chatroom_loop.item->chatroomid);
				debug = __LINE__;
				chatclient::send_message_to_clients(&the_websocket->chatclients,new_message);
				debug = __LINE__;
				message::dereference(&new_message);				
				debug = __LINE__;
				// Delete this chatroom.
				debug = __LINE__;
				the_websocket->chatrooms.remove(chatroom_loop.item);
				debug = __LINE__;
				chatroom_loop.row->used = false;				
				debug = __LINE__;
				delete chatroom_loop.item;
				debug = __LINE__;
			}
			debug = __LINE__;
			chatroom_loop.movenext();
		}
	}
	debug = __LINE__;
	// Remove this connection from the list of connection.
	the_websocket->chatclients.remove(_client);
	debug = __LINE__;
	// Clear any memory _client is using.
	_client->clear();
	return;
}

void task::receivedmessage()
{
	Debug debug(__FILE__,__func__,__LINE__);
	char first_letter;
	// Figure out the first letter of the message. This is to speed up finding a match.
	if ((_message != nullptr) && (_message->actual_message.length > 0)) {
		first_letter = _message->actual_message.data[0];
	} else {
		first_letter = ' ';
	}
	debug = __LINE__;
	do { // loop only once	
		if (chatcommand_getchatroomlist::processmessage(first_letter,_message,_client))
		{
			break; // Get the list of chatrooms.
		}
		debug = __LINE__;
		if (chatcommand_sendtoall::processmessage(first_letter,_message,_client))
		{
			break; // Send a message to all users. Server password is required.
		}
		debug = __LINE__;
		if (chatcommand_addchatroom::processmessage(first_letter,_message,_client))
		{
			break; // Add a chatroom. Server password is optional.
		}
		debug = __LINE__;
		if (chatcommand_adduser::processmessage(first_letter,_message,_client))
		{
			break; // Add a user. Server password is required.
		}
		debug = __LINE__;
		if (chatcommand_login::processmessage(first_letter,_message,_client))
		{
			break; // Log in as a certain user.
		}
		debug = __LINE__;
		if (chatcommand_logout::processmessage(first_letter,_message,_client))
		{
			break; // log out.
		}
		debug = __LINE__;
		if (chatcommand_joinchatroom::processmessage(first_letter,_message,_client))
		{
			break; // join chat room.
		}		
		debug = __LINE__;
		if (chatcommand_send::processmessage(first_letter,_message,_client))
		{
			break; // join chat room.
		}
		debug = __LINE__;
		if (chatcommand_leavechatroom::processmessage(first_letter,_message,_client))
		{
			break; // leave chat room.
		}
		debug = __LINE__;
		if (chatcommand_getsnaps::processmessage(first_letter,_message,_client))
		{
			break; // Start getting snaps.
		}
		debug = __LINE__;
		if (chatcommand_getusersinchatroom::processmessage(first_letter,_message,_client))
		{
			break; // get the list of users in the chatroom.
		}
		debug = __LINE__;
		if (chatcommand_removechatroom::processmessage(first_letter,_message,_client))
		{
			break; // removes a chatroom.
		}
		debug = __LINE__;
		if (chatcommand_sendto::processmessage(first_letter,_message,_client))
		{
			break; // Send a private message to a user.
		}
		debug = __LINE__;
		if (chatcommand_removeuser::processmessage(first_letter,_message,_client))
		{
			break; // Removes a user.
		}
		debug = __LINE__;
		if (chatcommand_sendsnap::processmessage(first_letter,_message,_client))
		{
			break; // Send a snap.
		}
		debug = __LINE__;
		if (chatcommand_nop::processmessage(first_letter,_message,_client))
		{
			break; // No operation.
		}
		debug = __LINE__;
		if (chatcommand_getusers::processmessage(first_letter,_message,_client))
		{
			break; // Gets the list of all users.
		}
		debug = __LINE__;
		if (chatcommand_getchatroomdetails::processmessage(first_letter,_message,_client))
		{
			break; // Gets the details of a chatroom.
		}		
		
		// Send a message back to the original client.
		// Message wasn't understood.
		debug = __LINE__;
		chatcommand::message_not_understood(_client);
	} while (false);
	debug = __LINE__;
	
	return;
	
}
