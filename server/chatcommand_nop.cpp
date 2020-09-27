#include "chatcommand_nop.h"
#include "websocket.h"
#include "datastring.h"

// nop()
// Does nothing. This helps the connection stay open.

bool chatcommand_nop::processmessage(char first_letter,message *received_message,chatclient *client)
{
	if ((first_letter != 'n') 
		|| (received_message->actual_message!="nop()")) {
		return false;
	}
	
	return true;
}
