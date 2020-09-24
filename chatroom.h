#ifndef __CHATROOM_H
#define __CHATROOM_H
#define MAX_USERS_IN_CHATROOM 50
#include "datablock.h"
#include "biglist.h"
#include "message.h"
#include <cstdint>

class user; // Forward declaration https://stackoverflow.com/questions/8526819/c-header-files-including-each-other-mutually
class chatclient;

class chatroom
{
	public:
	int64_t chatroomid;	
	datablock *name;
	datablock *question;
	datablock *answer;
	datablock *answer_hashed;
	bool delete_if_unused;
	biglist<chatclient *> clients;
	int number_of_clients;
	// Methods:
	int info(chatclient *client); // Some info describing this chatroom as a list of bitfields.
	int info(bool is_client_in_chatroom);
	void remove_client(chatclient *client);
	void add_client(chatclient *client);	
	chatroom();
	~chatroom();
	void clear();
	void initialize();	
	static chatroom *find(biglist<chatroom *>*chatroom_list,int64_t chatroom_id);
	static chatroom *find(biglist<chatroom *>*chatroom_list,datastring chatroom_name);
	void send_message_to_clients(message *message);
};
#endif
