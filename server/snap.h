#ifndef __SNAP_H
#define __SNAP_H
#include <time.h>
#include <inttypes.h>
#include "message.h"
#include "concurrent_queue.h"
class snap
{
	public:	
	snap(int64_t snap_id,message *item,int64_t fromuserid,int64_t touserid,time_t sentdate);
	~snap();
	int64_t to_userid;
	int64_t snapid;
	int64_t from_userid;
	message *data;
	time_t datesent;
	static const int snap_age = 172800; // two days.
	
	// Delete old snaps older than old_time. 
	// Assumes that snaps are put in time order with the oldest in the front.
	static void delete_old_snaps(concurrent_queue<snap *> &snaps,time_t old_time);
};
#endif
