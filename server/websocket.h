#ifndef __WEBSOCKET_H
#define __WEBSOCKET_H

#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "message.h"
#include "chatclient.h"
#include "biglist.h"
#include "datablock.h"
#include "user.h"
#include "chatroom.h"
#include "task.h"
#include "tasks.h"
#include "concurrent_queue.h"
#include "snap.h"

#define EXAMPLE_RX_BUFFER_BYTES (30)
#define EXAMPLE_RX_CHATROOM_BYTES 1024
struct payload
{
	unsigned char data[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
	size_t len;
};

class websocket
{
	public:
	websocket();
	~websocket();
	biglist<user *> users;
	biglist<chatroom *> chatrooms;
	biglist<chatclient *> chatclients;
	int64_t next_chatclientid;
	int64_t next_chatroomid;
	datablock *server_password;
	tasks *chatroom_tasks;
	tasks *slow_tasks;
	pthread_t task_thread;
	pthread_t slow_task_thread;
	concurrent_queue<snap *> snaps;
	int64_t next_snapid;
	static void *task_thread_routine(void *arg); // Routine that is called from a background thread if run_async is true.
	// The following three functions are libwebsockets callback functions.
	static int callback_http( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );
	static int callback_chatroom( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );
	static int callback_example( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );
};

extern websocket *the_websocket; // One global variable. Shhhh. Don't tell anyone.

enum protocols
{
	PROTOCOL_HTTP = 0,
	PROTOCOL_EXAMPLE,
	PROTOCOL_COUNT
};

static struct lws_protocols protocols[] =
{
	/* The first protocol must always be the HTTP handler */
	{
		"http-only",   /* name */
		websocket::callback_http, /* callback */
		0,             /* No per session data. */
		0,             /* max frame size / rx buffer */
	},
	{
		"example-protocol",
		websocket::callback_example,
		0, // size of client block.
		EXAMPLE_RX_BUFFER_BYTES,
	},
	{
		"chatroom-protocol",
		websocket::callback_chatroom,
		sizeof(chatclient),
		EXAMPLE_RX_CHATROOM_BYTES,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};
int main( int argc, char *argv[] );

#endif
