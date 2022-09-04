#ifndef UID_H
#define UID_H

#include <time.h>
#include <sys/types.h>
#include <unistd.h>



typedef struct uid ilrd_uid_t;
struct uid  /*  in header file */
{
	size_t counter; 
	pid_t pid;	 /*getpid (void) */
	time_t current_time; /*use ctime func */
};



/******************************************************************************************************************************************
DESCRIPTION
	create a uid.
ARGUMENTS
	non.
RETURN VALUE
	uid.
complexity - o(1).
*****************************************************************************************************************************************/
ilrd_uid_t UIDCreate();




/******************************************************************************************************************************************
DESCRIPTION
	checks of two uid's are the same.
ARGUMENTS
	uid1 - uid.
	uid2 - uid.
RETURN VALUE
	1 - equal.
	0 - not equal.
complexity - o(1).
*****************************************************************************************************************************************/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);


extern const ilrd_uid_t g_BadUid;


#endif /*UID_H*/

