#ifndef __TASK_H
#define __TASK_H
#include "chatclient.h"

// A task is something that needs to be done that will take a while.
// Tasks are done in a separate thread if websocket.run_async is true.
typedef bool (*fn_processmessage)(char,message *,chatclient *);

class task
{
	public:
	task();
	bool started;
	bool finished;
	bool running_async;	
	void closeconnection(chatclient *client,bool run_async);
	void receivedmessage(chatclient *client,message *new_message,bool run_async);
	void dotask();	
	fn_processmessage chatcommands[20];
	private:		
	enum task_type
	{
		close_connection = 1,
		received_message = 2
	};	
	task_type _task_type;
	chatclient *_client;
	message *_message;
	void initialize_task(task_type tasktype,bool run_async);
	void closeconnection();
	void receivedmessage();
	private:
	void addchatcommand(fn_processmessage next_command,int &index);
};
#endif
