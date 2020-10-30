#include "chatcommand_login.h"
#include "websocket.h"
#include "datastring.h"
#include "parameters.h"
#include "chatroom.h"
#include "readurl.h"
#include "Debug.h"

// login(messageid,username,password)
// Logs this connection in to the user.
// Sends a success or error message.

bool chatcommand_login::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'l') 
		|| (received_message->actual_message.substr(0,6)!="login(")) {
		return false;
	}
	
	Debug debug(__FILE__,__func__,__LINE__);
	user *new_user;
	datastring method_parameters;
	parameters parameters_parsed;
	user *logged_in_user;
	bool parameter_success = true;
	stringbuilder output;
	datastring username;
	datastring password;
	int64_t messageid;
	task *slow_task;	
	
	debug = __LINE__;
	method_parameters = received_message->actual_message.substr(6,received_message->actual_message.length-7);
	parameters_parsed.long_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	parameters_parsed.string_parameter(method_parameters,parameter_success);
	if (parameter_success) {		
		debug = __LINE__;
		messageid = parameters_parsed.long_parameters[0];
		username = parameters_parsed.string_parameters[1];
		password = parameters_parsed.string_parameters[2];
		
		debug = __LINE__;
		logged_in_user = user::find(&the_websocket->users,username);
		if (logged_in_user == nullptr) {
			// This user isn't found in ram.
			// Check the remote connection.
			// This is a slow task. Add it to the slow task queue.
			debug = __LINE__;
			slow_task = new task();
			slow_task->remotelogin(messageid,client,username,password,the_websocket->run_async);
			the_websocket->slow_tasks->add_task(slow_task);
			debug = __LINE__;
			if (!the_websocket->run_async) {
				the_websocket->slow_tasks->do_tasks();
			}
		} else {
			debug = __LINE__;
			login(messageid,client,logged_in_user,password,true);
		}
		return true;		
	} else {
		debug = __LINE__;
		parameters_not_correct(client);
	}
	debug = __LINE__;
	return true;
}

void chatcommand_login::login(int64_t messageid,chatclient *client,user *logged_in_user,datastring &password,bool fast_queue)
{
	Debug debug(__FILE__,__func__,__LINE__);
	datastring error_message;
	message *new_message;
			
	if (*logged_in_user->password != password) {
		debug = __LINE__;
		error_message = "Incorrect login.";
		error(client,error_message,messageid,fast_queue);
	} else {
		debug = __LINE__;
		client->logged_in_user = logged_in_user;
		if (client->chatrooms != nullptr) {
			// Send a message to all chatrooms that this user logged in.
			debug = __LINE__;
			biglist_iterator<chatroom *>chatroom_loop(client->chatrooms);
			while (!chatroom_loop.eof()) {
				// Send a message to all clients that this user logged in.
				debug = __LINE__;
				new_message = userjoinedchatroom(messageid,chatroom_loop.item->chatroomid,client->chatclientid,logged_in_user,chatroom_loop.item->number_of_clients);
				chatclient::send_message_to_clients(&chatroom_loop.item->clients,new_message,fast_queue);
				message::dereference(&new_message);
				chatroom_loop.movenext();
			}
		}
		debug = __LINE__;
		success_message(client,messageid,fast_queue);
	}
}

datablock *chatcommand_login::build_login_url(datastring &username,datastring &password)
{
	stringbuilder webpage;
	datablock *url;
	
	webpage += "http://www.chatscribble.com/API_CheckLogin.chp?UserName=";
	webpage.add_encodeURIComponent(username);
	webpage += "&PasswordHashed=";
	webpage.add_encodeURIComponent(password);	
	url = new datablock(webpage.length());
	webpage.tostring(url->data);
	return url;
}

void chatcommand_login::remotelogin(int64_t messageid,chatclient *client,datastring &username,datastring &password)
{
	Debug debug(__FILE__,__func__,__LINE__);
	const char *error_message;
	datastring error_string;
	datablock *url;
	char ch;
	int64_t userid;
	user *new_user;
	datablock *url_result;
	
	url = build_login_url(username,password);
	//printf("webpage url: %s\n",url->data); // debugging.
	debug = __LINE__;	
	url_result = readurl::read_url2(url->data,&error_message);
	debug = __LINE__;
	if (error_message != nullptr) {
		debug = __LINE__;
		//printf("Url results: %s\n",error_message); // debugging		
		error_string = error_message;
		error(client,error_string,messageid,false);
	} else {
		debug = __LINE__;
		//printf("Url results: %s\n",url_result->data); // debugging		
		debug = __LINE__;
		ch = url_result->data[0];
		if ((ch >= '0') && (ch <= '9')) {
			// Add this user.
			debug = __LINE__;
			new_user = new user();
			new_user->userid = atol(url_result->data);
			new_user->username = new datablock(username);
			new_user->password = new datablock(password);
			the_websocket->users.add(new_user);
			// Log this user in.
			debug = __LINE__;
			client->logged_in_user = new_user;
			new_user = nullptr;
			debug = __LINE__;
			success_message(client,messageid,false);
		} else {
			debug = __LINE__;
			error(client,*url_result,messageid,false);
		}
	}
	debug = __LINE__;
	datablock::dereference(&url);
	datablock::dereference(&url_result);
	return;
}
