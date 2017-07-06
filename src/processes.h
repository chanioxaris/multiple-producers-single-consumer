#ifndef _PROCESSES_
#define _PROCESSES_

union semun
	{
	int val;
	struct semid_ds *buf;
	unsigned short * array;
	};

struct memory 
	{
    char in_ds[1024];
	char out_ds[1024];
	};

#endif