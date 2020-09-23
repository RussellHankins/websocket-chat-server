#include "chatcommand.h"
#include "stringbuilder.h"
#include "message.h"
#include "datastring.h"
#include <stdio.h>
#include <inttypes.h>
#include "Debug.h"

void chatcommand::incorrect_system_password(chatclient *client,int64_t messageid)
{
	message *new_message;
	stringbuilder output;
		
	output += "error(26,Incorrect system password";
	output += messageid;
	output += ")";
	new_message = new message();
	*new_message = output;
	client->add_message(new_message);
	message::dereference(&new_message);
	client->callback_on_writable();
	return;
}

void chatcommand::success_message(chatclient *client,int64_t messageid)
{
	stringbuilder output;
	message *new_message;
	new_message = new message();
	output += "success(";
	output += messageid;
	output += ")";
	*new_message = output;		
	client->add_message(new_message);
	message::dereference(&new_message);
	client->callback_on_writable();
	return;
}

void chatcommand::not_implemented(chatclient *client)
{
	client->add_message("error(36,That command is not yet implemented.)");
	client->callback_on_writable();
	return;
}

void chatcommand::parameters_not_correct(chatclient *client)
{
	client->add_message("error(27,Syntax error in parameters.)");
	client->callback_on_writable();
	return;
}

void chatcommand::message_not_understood(chatclient *client)
{
	client->add_message("error(32,That message was not understood.)");
	client->callback_on_writable();
	return;
}

void chatcommand::error(chatclient *client,datastring error_message,int64_t messageid)
{
	message *new_message;	
	stringbuilder output;
	new_message = new message();
	output += "error(";
	output.addparameter(error_message);
	output.addparameter(messageid);
	output +=")";
	
	*new_message = output;
	client->add_message(new_message);
	message::dereference(&new_message);
	client->callback_on_writable();
}
void chatcommand::error(chatclient *client,datastring error_message)
{
	stringbuilder output;
	message *new_message;
	output += "error(";
	output.addparameter(error_message);
	output +=")";
	new_message = new message();
	*new_message = output;
	client->add_message(new_message);
	message::dereference(&new_message);
	client->callback_on_writable();
}

message *chatcommand::userjoinedchatroom(
	int64_t messageid,int64_t chatroomid,int64_t chatclientid
	, user *new_user,int number_of_clients)
{
	message *new_message;
	stringbuilder output;
	output += "userjoinedchatroom(";
	output.addparameter(messageid);	
	output.addparameter(chatroomid);
	output.addparameter(chatclientid);
	if (new_user == nullptr) {
		output += "0,0,";
	} else {
		output.addparameter(new_user->userid);
		output.addparameter(*new_user->username);
	}
	output.addparameter(number_of_clients);
	output += ")";
	new_message = new message();
	*new_message = output;
	return new_message;
}

message *chatcommand::userleftchatroom(int64_t messageid
,int64_t chatroomid, int64_t chatclientid, user *old_user,
int number_of_clients)
{
	message *new_message;
	stringbuilder output;
	output += "userleftchatroom(";
	output.addparameter(messageid);
	output.addparameter(chatroomid);
	output.addparameter(chatclientid);
	if (old_user == nullptr) {
		output += "0,0,";
	} else {
		output.addparameter(old_user->userid);
		output.addparameter(*old_user->username);
	}
	output.addparameter(number_of_clients);
	output += ")";
	new_message = new message();
	*new_message = output;
	return new_message;
}

message *chatcommand::chatroomwasdeleted(int64_t messageid,int64_t chatroomid)
{
	Debug debug(__FILE__,__func__,__LINE__);	
	message *new_message;
	stringbuilder output;
		
	output += "chatroomwasdeleted(";
	debug = __LINE__;
	output.addparameter(messageid);
	debug = __LINE__;
	output.addparameter(chatroomid);
	debug = __LINE__;
	output += ")";
	debug = __LINE__;		
	new_message = new message();
	*new_message = output;
	debug = __LINE__;
	return new_message;
}
message *chatcommand::chatroomcreated(int64_t messageid,int64_t chatroomid,datastring room_name,int info,int number_of_clients)
{
	stringbuilder output_message;
	message *new_message;
	output_message += "chatroomcreated(";		
	output_message.addparameter(messageid);
	output_message.addparameter(chatroomid);
	output_message.addparameter(room_name);
	output_message.addparameter(info);
	output_message.addparameter(number_of_clients);
	output_message += ")";
	
	new_message = new message();
	*new_message = output_message;
	return new_message;
}

message *chatcommand::yourchatclientid(int64_t chatclientid)
{
	stringbuilder output_message;
	message *new_message;
	output_message += "yourchatclientid(";		
	output_message.addparameter(chatclientid);
	output_message += ")";
	
	new_message = new message();
	*new_message = output_message;
	return new_message;
}

message *chatcommand::chat(int64_t messageid,int64_t chatroomid
,int64_t chatclientid,int64_t userid,datastring message_to_send)
{
	stringbuilder output_message;
	message *new_message;
	
	output_message += "chat(";		
	output_message.addparameter(messageid);
	output_message.addparameter(chatroomid);
	output_message.addparameter(chatclientid);
	output_message.addparameter(userid);
	output_message.addparameter(message_to_send);
	output_message += ")";
	
	new_message = new message();
	*new_message = output_message;
	return new_message;
}

message *chatcommand::msg(int64_t messageid
	,int64_t chatclientid,int64_t userid,datastring message_to_send)
{
	message *new_message;
	stringbuilder output_message;
	output_message += "msg(";
	output_message.addparameter(messageid);
	output_message.addparameter(chatclientid);
	output_message.addparameter(userid);
	output_message.addparameter(message_to_send);
	output_message += ")";
	new_message = new message();
	*new_message = output_message;
	return new_message;
}
