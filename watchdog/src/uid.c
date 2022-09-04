/*
dev: Daniel Gabay
rev: Alon Levy
status: approved
date: 17.1.22
*/

#include <stdatomic.h> /* atomic_size_t */

#include "uid.h"

const ilrd_uid_t g_BadUid = {0, 0, 0};
atomic_size_t count = 0;

ilrd_uid_t UIDCreate()
{
	ilrd_uid_t uid;
	++count;
	uid.counter = count;
	uid.pid = getpid();
	uid.current_time = time(NULL);
	return uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return ((uid1.counter == uid2.counter) && (uid1.pid == uid2.pid) && (uid1.current_time == uid2.current_time));
}
