#ifndef __SNAP_H
#define __SNAP_H
#include <time.h>
#include <inttypes.h>
#include "message.h"
#include "concurrent_queue.h"
class snap
{
	public:	
	snap(int64_t snap_id,message *item,int64_t fromuserid,int64_t touserid,time_t sentdate,time_t expiredate,int64_t read_time);
	~snap();
	int64_t to_userid;
	int64_t snapid;
	int64_t from_userid;
	message *snap_message;
	time_t datesent;
	time_t datereceived;
	time_t dateexpired;
	int64_t readtime;
	
	bool set_datereceived(time_t datereceived);
	// Delete old snaps older than old_time. 
	// Assumes that snaps are put in time order with the oldest in the front.
	static void delete_old_snaps(concurrent_queue<snap *> &snaps,time_t old_time);
	static snap *findsnap(concurrent_queue<snap *> &snaps,int64_t snapid);
};
#endif
