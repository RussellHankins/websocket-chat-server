#ifndef __TASK_H
#define __TASK_H
#include "chatclient.h"
#include <inttypes.h>

// A task is something that needs to be done that will take a while.
// Tasks are done in a separate thread if websocket.run_async is true.
class task
{
	public:
	task();
	bool started;
	bool finished;
	bool running_async;	
	void closeconnection(chatclient *client,bool run_async);
	void receivedmessage(chatclient *client,message *new_message,bool run_async);
	void remotelogin(int64_t messageid,chatclient *client,datastring username,datastring password,bool run_async);
	void dotask();	
	private:		
	enum task_type
	{
		close_connection = 1,
		received_message = 2,
		remote_login = 3
	};	
	task_type _task_type;
	chatclient *_client;
	datablock *_username;
	datablock *_password;
	message *_message;
	int64_t _messageid;
	void initialize_task(task_type tasktype,bool run_async);
	void closeconnection();
	void receivedmessage();	
};
#endif
