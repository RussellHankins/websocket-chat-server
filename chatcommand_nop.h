#ifndef __CHATCOMMAND_NOP_H
#define __CHATCOMMAND_NOP_H
#include "chatcommand.h"
#include "message.h"
#include "chatclient.h"

// nop()
// No operation. This helps keep connections open.
class chatcommand_nop:chatcommand
{
	public:
	static bool processmessage(char first_letter,message *received_message,chatclient *client);
};
#endif
