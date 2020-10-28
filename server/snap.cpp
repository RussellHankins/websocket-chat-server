#include "snap.h"
#include <inttypes.h>
#include <string.h>
snap::snap(int64_t snap_id,message *item,int64_t fromuserid,int64_t touserid
,time_t sentdate,time_t expiredate,int64_t read_time)
{
	snapid = snap_id;
	snap_message = item->clone();
	to_userid = touserid;
	from_userid = fromuserid;
	datesent = sentdate;
	datereceived = 0;
	dateexpired = expiredate;
	readtime = read_time;
}
snap::~snap()
{
	message::dereference(&snap_message);
}

void snap::delete_old_snaps(concurrent_queue<snap *> &snaps,time_t current_time)
{
	// Delete old snaps.
	// Snaps are deleted by setting the item to null in the snaps queue.
	// Also, the snap_message->actual_message.length is set to 0 
	// just in case it's in a queue to be sent already.
	snap *current_snap;
	while (snaps.peek(current_snap))
	{		
		if ((current_snap != nullptr) 
		&& (current_snap->dateexpired > current_time)) {
			// Stop looking for snaps to pop off the queue.
			break;
		}
		// Remove this snap from the queue. Delete if necessary.
		if (snaps.dequeue(current_snap)) {
			if (current_snap != nullptr) {
				delete current_snap;
			}
		}
	}
	// Loop through all the snaps to delete the data of items that have expired.
	snaps.movefirst();
	while (snaps.current != nullptr) {
		current_snap = snaps.current->item;
		// if this item hasn't been deleted then
		if ((current_snap != nullptr) 
		&& (current_snap->snap_message != nullptr) 
		&& (current_snap->snap_message->actual_message.length > 0)) {
			// If this item is expired then
			if (((current_snap->dateexpired == 0) && (current_snap->datereceived > 0))
			|| ((current_snap->dateexpired > 0) && (current_snap->dateexpired <= current_time))) {
				// Marks this message as empty so it doesn't get sent out.
				// There could be a copy/second reference to the message in the client's queue.
				current_snap->snap_message->actual_message.length = 0;
				// Delete this item.
				delete current_snap;
				// Mark it as deleted in the snaps queue.
				snaps.current->item = nullptr;
			}
		}
		snaps.movenext();
	}	
}

snap *snap::findsnap(concurrent_queue<snap *> &snaps,int64_t snapid)
{
	snap *current_snap;
	snaps.movefirst();
	while (snaps.current != nullptr) {
		current_snap = snaps.current->item;
		if ((current_snap != nullptr) && (current_snap->snapid == snapid)) {
			return current_snap;
		}
		snaps.movenext();
	}
	return nullptr;
}

bool snap::set_datereceived(time_t datereceived)
{
	// The date received is between the 4th and 5th comma.
	int fourth_comma = 0;
	int fifth_comma = 0;
	int comma_count = 0;
	int loop;	
	int length = snap_message->actual_message.length;
	char *actual_message = snap_message->actual_message.data;
	const int number_buffer_size = 100;
	char number_buffer[number_buffer_size];
	int number_buffer_length;
	for(loop=0;loop<length;loop++) {
		if (actual_message[loop] == ',') {
			comma_count++;
			if (comma_count == 4) {
				fourth_comma = loop;
			} else if (comma_count == 5) {
				fifth_comma = loop;
				break;
			}
		}
	}
	if (fifth_comma == 0) {
		return false;
	}
	number_buffer_length = snprintf(number_buffer,number_buffer_size,"%" PRId64,(int64_t)datereceived);
	if (number_buffer_length >= number_buffer_size) {
		return false;
	}
	if (fifth_comma - fourth_comma - 1 < number_buffer_length) {
		return false;
	}
	for(loop=fourth_comma + 1;loop<fifth_comma;loop++) {
		actual_message[loop] = ' ';
	}
	memcpy(actual_message + fifth_comma - number_buffer_length,number_buffer,number_buffer_length);
	return true;
}
