#ifndef __CHATCLIENT_H
#define __CHATCLIENT_H
#include "biglist.h"
#include "message.h"
#include "concurrent_queue.h"
#include <libwebsockets.h>

//#include <queue>

class user;
class chatroom;

class chatclient
{
	public:
	int64_t chatclientid; // Unuqie number assigned as connections come in.
	user *logged_in_user; // Optional. The user this client is logged in as.
	biglist<chatroom *> *chatrooms;  // What chatrooms this client is using.
	concurrent_queue<message *> *messages_to_send;
	bool should_disconnect;	
	int64_t latest_snapid;
	struct lws *wsi; // From libwebsockets callback.
	void add_chatroom(chatroom *room); // Add a chatroom to the list of chatrooms above.
	void remove_chatroom(chatroom *room); // Remove a chatroom from the list of chatrooms above.
	void push_message(message **message_to_add); // Adds a message to messages_to_send. Sets *message_to_add to nullptr. Also calls callback_on_writable.
	void add_message(message *message_to_add); // Adds a message to messages_to_send. Increments message_to_add->usage.
	void add_message(datastring message_to_add);
	void add_message(datastring *message_to_add);
	void add_message(const char *message_to_add);
	void disconnect();
	message *get_next_message(); // Returns a message if it's available. Delete the returned message with message::dereference.
	void initialize(struct lws *opened_wsi,int64_t chat_client_id);	// Call this when the connection is opened.
	void clear(); // Call this when the connection is closed.	
	void send_yourchatclientid();
	int callback_on_writable(); // Signal libwebsockets that there's data to send. This can be called from a different thread.
	// Send a message to a list of clients.
	static void send_message_to_clients(biglist<chatclient *> *clients,message *message);
	static void send_message_to_user(biglist<chatclient *> *clients,message *message,int64_t userid,bool this_is_snap_message);
};
#endif
