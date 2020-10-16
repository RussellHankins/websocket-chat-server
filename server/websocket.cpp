#include "websocket.h"
#include "biglist.h"
#include "test.h"
#include "Debug.h"
#include "datastring.h"
#include "datablock.h"
#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


// Ok. Maybe 2 global variables. Shhhh. Don't tell anyone.
websocket *the_websocket = nullptr;
struct payload received_payload;
bool run_async = true; // Set to false for testing.

websocket::websocket()
{
	server_password = nullptr;
	chatroom_tasks = new tasks();
	next_chatclientid = 1;
	next_chatroomid = 1;
}
websocket::~websocket()
{
	if (server_password != nullptr) {
		delete server_password;
		server_password = nullptr;
	}
	users.clear(true);
	chatrooms.clear(true);
	chatclients.clear(true);
	delete chatroom_tasks;
	chatroom_tasks = nullptr;
}

int websocket::callback_http( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
	switch( reason )
	{
		case LWS_CALLBACK_HTTP:
			lws_serve_http_file( wsi, "example.html", "text/html", NULL, 0 );
			break;
		default:
			break;
	}

	return 0;
}

int websocket::callback_chatroom( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
	Debug debug(__FILE__,__func__,__LINE__);
	chatclient *client = (chatclient *)user;	
	task *task_item;
	message *new_message;
	int return_value = 0;
	
	debug = __LINE__;
	switch( reason )
	{
		case LWS_CALLBACK_ESTABLISHED:
			// (VH) after the server completes a handshake with an incoming
			// client.  If you built the library with ssl support, in is a
			// pointer to the ssl struct associated with the connection or NULL.
			//printf("LWS_CALLBACK_ESTABLISHED\n");
			debug = __LINE__;
			//if (client->wsi != wsi) {
				client->initialize(wsi,the_websocket->next_chatclientid++);
				the_websocket->chatclients.add(client);
				client->send_yourchatclientid();
			//}
			debug = __LINE__;
			break;
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			//printf("LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
			//datastring error;
			//error.data = (char *)in;
			//error.length = len;
			//error.println();
			
			break;
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			// after your client connection completed
			// a handshake with the remote server.
			// Your client connection is actually active 
			// only when you receive LWS_CALLBACK_CLIENT_ESTABLISHED for it.
			//printf("LWS_CALLBACK_CLIENT_ESTABLISHED\n");
			debug = __LINE__;
			/*if (client->wsi != wsi) {
				printf("Initialized\n");
				client->initialize(wsi,the_websocket->next_chatclientid++);
				the_websocket->chatclients.add(client);
				client->send_yourchatclientid();
			}
			debug = __LINE__;*/
			break;
			
		case LWS_CALLBACK_CLOSED:
			debug = __LINE__;
			task_item = new task();	
			task_item->closeconnection(client,run_async);			
			the_websocket->chatroom_tasks->add_task(task_item);
			task_item = nullptr;
			if (!run_async) {
				the_websocket->chatroom_tasks->do_tasks();
			}
			debug = __LINE__;
			break;
			
		case LWS_CALLBACK_RECEIVE:
			debug = __LINE__;
			if (client->wsi != nullptr) {
				new_message = new message();
				if (!new_message->set((char *)in, len)) {
					printf("Memory error.\n");
				} else {
					task_item = new task();				
					task_item->receivedmessage(client,new_message,run_async);
					debug = __LINE__;
					the_websocket->chatroom_tasks->add_task(task_item);
					debug = __LINE__;
					if (!run_async) {
						the_websocket->chatroom_tasks->do_tasks();
					}
				}
				message::dereference(&new_message);
			}
			debug = __LINE__;
			//lws_callback_on_writable_all_protocol( lws_get_context( wsi ), lws_get_protocol( wsi ) );
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			//printf("LWS_CALLBACK_SERVER_WRITEABLE\n");
			// Try to get a message. Send it if it's available.
			debug = __LINE__;
			if (client->wsi != nullptr) {
				new_message = client->get_next_message();
				if (new_message == nullptr) {
					if (client->should_disconnect) {
						return_value = -1;
					}
				} else {
					// Send the message.
					debug = __LINE__;
					lws_write( wsi, (unsigned char *)new_message->actual_message.data,new_message->actual_message.length,LWS_WRITE_TEXT);
					debug = __LINE__;
					message::dereference(&new_message);
					// If there are more messages, send a request to send another message.
					if ((client->should_disconnect) 
					|| ((client->messages_to_send != nullptr)
					&& (!client->messages_to_send->empty()))) {
						lws_callback_on_writable(client->wsi);
					}
				}
			}
			debug = __LINE__;
			break;
			
		default:
			break;
	}
	// When you want to close a connection, you do it by returning -1 from a callback for that connection.
	return return_value;
}

int websocket::callback_example( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{	
	Debug debug(__FILE__,__func__,__LINE__);
	switch( reason )
	{
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			// after your client connection completed
			// a handshake with the remote server.
			// Your client connection is actually active 
			// only when you receive LWS_CALLBACK_CLIENT_ESTABLISHED for it.
			break;
		case LWS_CALLBACK_ESTABLISHED:
			// (VH) after the server completes a handshake with an incoming
			// client.  If you built the library with ssl support, in is a
			// pointer to the ssl struct associated with the connection or NULL.*/			 
			break;
		case LWS_CALLBACK_CLOSED: 
			// when the websocket session ends			
			break;
		case LWS_CALLBACK_RECEIVE:
			// data has appeared for this server endpoint from a
			// remote client, it can be found at *in and is
			// len bytes long 
			debug = __LINE__;
			memcpy( &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], in, len );
			received_payload.len = len;
			lws_callback_on_writable_all_protocol( lws_get_context( wsi ), lws_get_protocol( wsi ) );
			debug = __LINE__;
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
			//  If you call lws_callback_on_writable() on a connection, you will
			// get one of these callbacks coming when the connection socket
			// is able to accept another write packet without blocking.
			// If it already was able to take another packet without blocking,
			// you'll get this callback at the next call to the service loop
			// function.  Notice that CLIENTs get LWS_CALLBACK_CLIENT_WRITEABLE
			// and servers get LWS_CALLBACK_SERVER_WRITEABLE.
			debug = __LINE__;
			lws_write( wsi, &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], received_payload.len, LWS_WRITE_TEXT );
			debug = __LINE__;
			break;
		case LWS_CALLBACK_PROTOCOL_INIT:
			// One-time call per protocol, per-vhost using it, so it can
			// do initial setup / allocations etc
			debug = __LINE__;
			break;
		default:
			break;
	}
	debug = __LINE__;
	// When you want to close a connection, you do it by returning -1 from a callback for that connection.
	return 0;
}

void *websocket::task_thread_routine(void *arg)
{
	// Start of error handling.
	pid_t tid = error_signals::GetThreadID();
	error_signals *error_thread = error_signals::GetThread(tid);
	if (error_thread == nullptr)
	{
	  error_thread = error_signals::AddThread(tid);
	}
	volatile int val = 0;
	if (error_thread != nullptr)
	{
	  error_thread->LineNumberStack = 0;
	  val = setjmp(error_thread->position);
	}
	if (val != 0)
	{
	  error_thread->DisplayErrorMessage(val);
	  // Exit the program.
	  error_signals::RemoveHandlers();
	  exit(val);
	}
	// End of error handling.
	Debug debug(__FILE__,__func__,__LINE__);
	tasks *chatroom_tasks = (tasks *)arg;	
	
	while (true) { // In the future, there might be a kill command to exit this loop.
		debug = __LINE__;
		chatroom_tasks->wait_for_tasks(1);
		debug = __LINE__;
		chatroom_tasks->do_tasks();
		debug = __LINE__;
	}
	error_thread->ReleaseThread(tid); // Error thread cleanup.
	pthread_exit(NULL);	 // This is how threads exit.
}

int main( int argc, char *argv[] )
{	
	// Start of error handling.	
	// This is error handling to display the stack trace of a GPF error.
	// This code has to be done at the beginning of each thread.
	error_signals::AddHandlers();
    pid_t tid = error_signals::GetThreadID();
    error_signals *error_thread = error_signals::GetThread(tid);
    if (error_thread == nullptr)
    {
      error_thread = error_signals::AddThread(tid);
    }
    volatile int val = 0;
    if (error_thread != nullptr)
    {
      error_thread->LineNumberStack = 0;
      val = setjmp(error_thread->position);
    }
    if (val != 0)
    {
	  // This code runs if there's an error.
      error_thread->DisplayErrorMessage(val); // Display a stack trace like C#.
      error_signals::RemoveHandlers();
	  return val;
    }
	// End of error handling.
	Debug debug(__FILE__,__func__,__LINE__);
	
	the_websocket = new websocket();
	debug = __LINE__;
	int port;
	if (argc != 3) {
		printf("%s Port ServerPassword\n",argv[0]);
		printf("  starts the server listening to Port.\n");
		printf("  ServerPassword is the password to edit users/chatrooms.\n");
		printf("%s test classname\n",argv[0]);
		printf("  Runs the test for classname.\n");
		printf("%s test testnumber\n",argv[0]);
		printf("  testnumber can be from 1 to %d.\n",test::testcount());
		printf("%s test all\n",argv[0]);
		printf("  Runs all the tests.\n");
		printf("You can add extra optional parameters of file locations.\n");
		printf("  File commands require a file location.\n");
		printf("  The file locations are numbered from 1 to N\n");
		printf("  where N is the number of file locations specified.\n");
		return 1;
	}
	debug = __LINE__;
	if (strcmp(argv[1],"test")==0) {
		port = test::runtest(argv[2]);
		return port;
	}
	debug = __LINE__;
	port = atoi(argv[1]);
	debug = __LINE__;
	if ((port <=0) || (port > 65535)) {
		printf("Error: Port must be between 1 and 65535.\n");
		return 2;
	}
	
	debug = __LINE__;
	struct lws_context_creation_info info;
	memset( &info, 0, sizeof(info) );

	info.port = port;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;
	
	debug = __LINE__;
	the_websocket->server_password = new datablock(argv[2]);
	debug = __LINE__;
	struct lws_context *context = lws_create_context( &info );
	debug = __LINE__;
	if (run_async) {
		// Start a separate thread for tasks.
		if (pthread_create(&the_websocket->task_thread,NULL,websocket::task_thread_routine,the_websocket->chatroom_tasks) != 0) {
			// Thread was unable to be created.
			run_async = false;
		}
	}
	debug = __LINE__;
	while( 1 )
	{
		lws_service( context, /* timeout_ms = */ 1000000 );
	}
	debug = __LINE__;
	lws_context_destroy( context );
	delete the_websocket;
	error_thread->ReleaseThread(tid); // Error thread cleanup.
	//ZZZ
	return 0;
}

