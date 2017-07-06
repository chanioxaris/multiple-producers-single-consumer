#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processes.h"

int main(int argc, char* argv[])
	{
	int i, N, K;		
				
	if (argc != 5)
		{
		printf("Wrong input arguments! \n");
		return -1;
		}	
	
	for (i = 1 ; i < (argc-1) ; i++)
		{
		if (!strcmp(argv[i], "-N"))
			{
			N = atoi(argv[++i]);
			continue;
			}
		if (!strcmp(argv[i], "-K"))
			{
			K = atoi(argv[++i]);
			continue;
			}
		printf("Wrong input arguments! \n");
		return -1;
		}
		
	int sum_pid_match = processes(N, K);
	
	
	printf("\n");
	printf("\n");
	printf("\n");
	printf("The number of P processes is: %d\n", N);
	printf("The number of steps is: %d\n", K);
	printf("The sum of pid_matches is: %d\n", sum_pid_match);
	return 0;
	}