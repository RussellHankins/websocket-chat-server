#include "snap.h"
snap::snap(int64_t snap_id,message *item,int64_t fromuserid,int64_t touserid,time_t sentdate)
{
	snapid = snap_id;
	data = item->clone();
	to_userid = touserid;
	from_userid = fromuserid;
	datesent = sentdate;
}
snap::~snap()
{
	message::dereference(&data);
}

void snap::delete_old_snaps(concurrent_queue<snap *> &snaps,time_t old_time)
{
	snap *item;
	while (snaps.peek(item))
	{
		if (item->datesent >= old_time) {
			break;
		}
		if (snaps.dequeue(item)) {
			delete item;
		}
	}
}


