#include "chatroom.h"
#include "chatclient.h"
#include "user.h"

chatroom::chatroom()
{
	initialize();
}
chatroom::~chatroom()
{
	datablock::dereference(&name);
	datablock::dereference(&question);
	datablock::dereference(&answer_hashed);
	clear();
}
void chatroom::clear()
{
	clients.clear(true);
	return;
}
void chatroom::initialize()
{
	chatroomid = 0;
	name = nullptr;
	question = nullptr;
	answer_hashed = nullptr;
	delete_if_unused = true;
	number_of_clients = 0;
	return;
}
chatroom *chatroom::find(biglist<chatroom *>*chatroom_list,int64_t chatroom_id)
{
	biglist_iterator<chatroom *> loop(chatroom_list);
	while (!loop.eof()) {
		if (loop.item->chatroomid == chatroom_id) {
			return loop.item;
		}
		loop.movenext();
	}	
	return nullptr;
}
chatroom *chatroom::find(biglist<chatroom *>*chatroom_list,datastring chatroom_name)
{
	biglist_iterator<chatroom *> loop(chatroom_list);
	while (!loop.eof()) {
		if (*loop.item->name == chatroom_name) {
			return loop.item;
		}
		loop.movenext();
	}	
	return nullptr;
}

void chatroom::remove_client(chatclient *client)
{	
	if (clients.remove(client)) {
		number_of_clients--;
	}
	return;
}
void chatroom::add_client(chatclient *client)
{
	if (clients.add(client)) {
		number_of_clients++;
	}
	return;
}
void chatroom::send_message_to_clients(message *message)
{
	chatclient::send_message_to_clients(&clients,message,true);
}
int chatroom::info(chatclient *client)
{
	bool is_client_in_chatroom = false;
	biglist_iterator<chatclient *>loop(&clients);
	while (!loop.eof()) {
		if (loop.item->chatclientid == client->chatclientid) {
			is_client_in_chatroom = true;
			break;
		}
		loop.movenext();
	}
	return info(is_client_in_chatroom);
}
int chatroom::info(bool is_client_in_chatroom)
{
	//info is a number:
	//	bit 1 = Are you in this chatroom.
	//	bit 3 = Does this chatroom require a password.
	//	bit 4 = Will this chatroom be deleted when everybody leaves.
	int info = 0;
	if (is_client_in_chatroom) {
		info += 1;
	}	
	if ((answer_hashed != nullptr) && (answer_hashed->length > 0)) {
		info += 4;
	}
	if (delete_if_unused) {
		info += 8;
	}
	return info;
}
