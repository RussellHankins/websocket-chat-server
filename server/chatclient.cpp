#include "chatclient.h"
#include "chatcommand.h"
//#include "user.h"
//#include "chatroom.h"


void chatclient::initialize(struct lws *opened_wsi,int64_t chat_client_id)
{
	chatclientid = chat_client_id;
	logged_in_user = nullptr;
	chatrooms = nullptr;
	messages_to_send = nullptr;
	messages_to_send_from_slow_commands = nullptr;
	should_disconnect = false;
	wsi = opened_wsi;
	latest_snapid = -1;
}

void chatclient::clear()
{
	logged_in_user = nullptr;
	if (chatrooms != nullptr) {
		delete chatrooms;
		chatrooms = nullptr;
	}
	if (messages_to_send != nullptr) {
		clear_messages(messages_to_send);
		messages_to_send = nullptr;
	}
	if (messages_to_send_from_slow_commands != nullptr) {
		clear_messages(messages_to_send_from_slow_commands);
		messages_to_send_from_slow_commands = nullptr;
	}
	return;
}
void chatclient::add_chatroom(chatroom *room)
{
	if (chatrooms == nullptr) {
		chatrooms = new biglist<chatroom *>();
	}
	chatrooms->add(room);
	return;
}
void chatclient::remove_chatroom(chatroom *room)
{
	if (chatrooms != nullptr) {
		chatrooms->remove(room);
	}
	return;
}

void chatclient::push_message(message **message_to_add)
{
	// Adds a message to messages_to_send. 
	// Sets *message_to_add to nullptr. 
	// Also calls callback_on_writable.
	if (wsi != nullptr) {
		if (messages_to_send == nullptr) {
			messages_to_send = new concurrent_queue<message *>();
		}
		if (messages_to_send != nullptr) {
			messages_to_send->enqueue(*message_to_add);
			*message_to_add = nullptr;
		} else {
			message::dereference(message_to_add);
		}
		callback_on_writable();
	} else {
		message::dereference(message_to_add);
	}
	return;
}

void chatclient::add_message(message *message_to_add)
{
	add_message(message_to_add,true);
}

void chatclient::add_message(message *message_to_add,bool fast_queue)
{
	// Adds a message to messages_to_send or messages_to_send_from_slow_commands.
	// Increments message_to_add->usage.
	if (wsi != nullptr) {
		if (fast_queue) {
			if (messages_to_send == nullptr) {
				messages_to_send = new concurrent_queue<message *>();
			}
			if (messages_to_send != nullptr) {
				messages_to_send->enqueue(message_to_add->clone());
			}
		} else {
			if (messages_to_send_from_slow_commands == nullptr) {
				messages_to_send_from_slow_commands = new concurrent_queue<message *>();
			}
			if (messages_to_send_from_slow_commands != nullptr) {
				messages_to_send_from_slow_commands->enqueue(message_to_add->clone());
			}
		}
	}
	return;
}

bool chatclient::has_messages()
{
	if ((messages_to_send != nullptr) && (!messages_to_send->empty())) {
		return true;
	}
	return ((messages_to_send_from_slow_commands != nullptr) 
	&& (!messages_to_send_from_slow_commands->empty()));
}

message *chatclient::get_next_message()
{
	// Returns a message if it's available. 
	// Delete the returned message with message::dereference.
	message *next_message = nullptr;
	if ((messages_to_send != nullptr) && (!messages_to_send->empty())) {
		 if (!messages_to_send->dequeue(next_message)) {
			 next_message = nullptr;
		 }		
	}
	// If there's no fast message, look in the slow message queue.
	if ((next_message == nullptr) && (messages_to_send_from_slow_commands != nullptr)) {
		if (!messages_to_send_from_slow_commands->dequeue(next_message)) {
			next_message = nullptr;
		}
	}
	return next_message;
}

int chatclient::callback_on_writable()
{
	if (wsi == nullptr) {
		// If the client disconnected, then wsi is no longer valid.
		return 0;
	}
	return lws_callback_on_writable(wsi);
}
void chatclient::add_message(datastring *message_to_add)
{
	add_message(*message_to_add);
	return;
}
void chatclient::add_message(datastring message_to_add)
{
	if (wsi != nullptr) {
		message *new_message;
		new_message = new message();
		new_message->set(message_to_add.data,message_to_add.length);
		add_message(new_message);
		message::dereference(&new_message);
	}
	return;
}
void chatclient::add_message(const char *message_to_add)
{
	message *new_message;
	if (wsi != nullptr) {
		new_message = new message();
		new_message->set(message_to_add);
		add_message(new_message);
		message::dereference(&new_message);
	}
	return;
}
void chatclient::send_message_to_clients(biglist<chatclient *> *clients,message *message,bool fast_queue)
{
	// Add the message to all the queues.
	biglist_iterator<chatclient *>loop(clients);
	while (!loop.eof()) {
		loop.item->add_message(message,fast_queue);
		loop.item->callback_on_writable();
		loop.movenext();
	}
	// Tell libwebsockets that there are messages to send.
	loop.movefirst();
	while (!loop.eof()) {
		loop.item->callback_on_writable();
		loop.movenext();
	}	
	return;
}
void chatclient::send_message_to_user(biglist<chatclient *> *clients,message *message,int64_t userid,bool this_is_snap_message)
{
	chatclient *client;
	user *client_user;
	biglist_iterator<chatclient *>loop(clients);
	while (!loop.eof()) {
		client = loop.item;
		client_user = client->logged_in_user;
		if ((client_user != nullptr) && (client_user->userid == userid)) {
			if ((!this_is_snap_message) || (client->latest_snapid >= 0)) {
				client->add_message(message);
				client->callback_on_writable();
			}
		}
	}
	return;
}

void chatclient::disconnect()
{
	should_disconnect = true;
	callback_on_writable();
}

void chatclient::send_yourchatclientid()
{
	message *new_message;
	new_message = chatcommand::yourchatclientid(chatclientid);
	push_message(&new_message);
	return;
}

void chatclient::clear_messages(concurrent_queue<message *> *messages)
{
	message *message_to_delete;
	while (messages->dequeue(message_to_delete)) {
		message::dereference(&message_to_delete);
	}
	delete messages;
}
