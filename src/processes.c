#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "processes.h"

#define SHMSIZE 2048


void sem_down(int sem_id, int sem_number)
	{
    struct sembuf sem_oper;

    sem_oper.sem_num = sem_number;
    sem_oper.sem_op = -1;
    sem_oper.sem_flg = 0;
	
    if (semop(sem_id, &sem_oper, 1) != 0)
		printf("Semaphore operation failed! \n");
	}

void sem_up(int sem_id, int sem_number)
	{
    struct sembuf sem_oper;

    sem_oper.sem_num = sem_number;
    sem_oper.sem_op = 1;  
    sem_oper.sem_flg = 0;
    
	if (semop(sem_id, &sem_oper, 1) != 0)
		printf("Semaphore operation failed! \n");
	}

void sem_init(int sem_id)
	{
	union semun arg0;
	union semun arg1;
	
	arg0.val = 0;
	arg1.val = 1;
			
	if (semctl(sem_id, 0, SETVAL, arg1) == -1)
		{
		printf("semctl failed!\n");
		exit(1);	
		}
	
	if (semctl(sem_id, 1, SETVAL, arg0) == -1)
		{
		printf("semctl failed!\n");
		exit(1);	
		}
		
	if (semctl(sem_id, 2, SETVAL, arg1) == -1)
		{
		printf("semctl failed!\n");
		exit(1);	
		}
	
	if (semctl(sem_id, 3, SETVAL, arg0) == -1)
		{
		printf("semctl failed!\n");
		exit(1);	
		}
	}

int processes(number_p, steps)
	{
	int i, shmid, semid;
	pid_t pid;
	char string_to_edit[1000];
	struct memory *shmaddr;
		
	/*Create shared memory*/
	if ((shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | 0666)) == -1)
		{
		printf("shmget failed!\n");
		exit(1);
		}
	
	shmaddr = shmat(shmid, 0, 0);
	if (shmaddr == (struct memory *) -1)
		{
		printf("shmat failed!\n");
		exit(1);
		}	
	
	/*Create semaphores*/
	if ((semid = semget(IPC_PRIVATE, 4, IPC_CREAT | 0666)) == -1)
		{
		printf("semget failed!\n");
		exit(1);	
		}
	
	sem_init(semid);
	
	/*Create P processes*/
	for (i = 0 ; i < number_p ; i++)
		{			
		pid = fork();

		if (pid == 0)
			break;
		}	
		
	/*Parent process*/
	if (pid > 0)										
		{
		int j, status, sum = 0;
		char terminate_id[24];
					
		/*Read and write messages to shm for K times*/
		for (i = 0 ; i < steps ; i++)
			{
			/*Critical section (Process C reads from shared memory)*/
			sem_down(semid, 1);
			stpcpy(string_to_edit, (shmaddr->in_ds));		
			sem_up(semid, 0);	
			
			/*Capitalize string*/
			for(j = 0 ; j <= strlen(string_to_edit) ; j++)
				{
				if(islower(string_to_edit[j]))
					string_to_edit[j] = toupper(string_to_edit[j]);					
				}
							
			/*Critical section (Process C writes to shared memory)*/
			sem_down(semid, 2);
			stpcpy((shmaddr->out_ds), string_to_edit);
			sem_up(semid, 3);
			}	
										
		/*Create terminate signal*/
		sprintf(terminate_id, "%d ", -1);
		strcpy(string_to_edit, terminate_id);
		strcat(string_to_edit, "Terminate message");	
														
		/*Critical section (Process C writes terminate message to shared memory)*/
		sem_down(semid, 2);
		stpcpy((shmaddr->out_ds), string_to_edit);
		sem_up(semid, 3);			
			
		/*Wait for all child processes to terminate*/
		for (i = 0 ; i < number_p ; i++)
			{
			if (wait(&status) >= 0)
				{
				if (WIFEXITED(status))
					sum += WEXITSTATUS(status);
				}
			}	
			
		/*Remove semaphores*/
		if (semctl(semid, 0, IPC_RMID) == -1)
			{
			printf("semctl failed!\n");
			exit(1);	
			}
	
		/*Detach the shared memory*/
		if (shmdt(shmaddr) == -1)
			{
			printf("shmdt failed!\n");
			exit(1);
			}
			
		/*Remove the shared memory*/
		if ((shmctl(shmid, IPC_RMID, 0)) == -1)
			{
			printf("shmdt failed!\n");
			exit(1);
			}					
		return sum;			
		} 
		
	/*Child process*/	
	else if (pid == 0)															
		{
		int pid_match = 0, int_pid_check = 1, count, string_len, message_len, p_pid;	
		char buffer[1000], char_pid[24], char_pid_check[24];
		char *parent_message;
		
		/*Open file to read from*/
		FILE *fp = fopen("input.txt", "r");	
		
		if (!fp)
			{
			printf("fopen failed!\n");
			exit(1);	
			}		
			
		srand(time(NULL));
		p_pid = getpid();
		sprintf(char_pid, "%d ", p_pid);
						
		while (int_pid_check > 0)
			{	
			/*Read random line from text into buffer*/
			count = 0;
			
			while (fgets(buffer, 1000 , fp) != NULL)
				{
				count++;
				if ((rand() / (float)RAND_MAX) <= (1.0 / count)) 
					break;
				}			
				
			/*Add process's pid to message*/
			strcpy(string_to_edit, char_pid);
			strcat(string_to_edit, buffer);
			
			/*Critical section (Process P writes to shared memory)*/
			sem_down(semid, 0);
			stpcpy((shmaddr->in_ds), string_to_edit);
			sem_up(semid, 1);
				
			/*Critical section (Process P reads from shared memory)*/
			sem_down(semid, 3);
			stpcpy(string_to_edit, (shmaddr->out_ds));
			sem_up(semid, 2);
				
			/*Check	edited message from process C*/
			parent_message = strchr(string_to_edit, ' ');
			string_len = strlen(string_to_edit);
			message_len = strlen(parent_message);
			strncpy(char_pid_check, string_to_edit, (string_len - message_len));
			char_pid_check[string_len - message_len] = 0; //null terminate destination
			
			/*Checks message's pid, if same increase pid_match*/
			int_pid_check = atoi(char_pid_check);
			if (int_pid_check != p_pid && int_pid_check != -1)
				printf("Created by %d and received by %d:%s\n", atoi(char_pid_check), atoi(char_pid), parent_message);
			else if (int_pid_check == p_pid && int_pid_check != -1)
				{
				printf("Created by %d:%s\n", atoi(char_pid_check), parent_message);
				pid_match++;		
				}
			}		
		/*Restore semaphores, so other processes don't get stuck*/
		sem_up(semid, 0);
		sem_down(semid, 1);		
		sem_up(semid, 3);
		sem_down(semid, 2);
		
		fclose(fp);
		
		exit(pid_match);
		} 
	else 
		{
		printf("Failed to fork\n");
		exit(1);
		}	
	}