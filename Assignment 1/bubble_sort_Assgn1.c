#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

int main()
{
        pid_t pid, p;
	int *num;
	int j;
	int i;

	int shmid, status;
	int *a, *b;
	int *arr;
	int n, pass;
	int sum=0;
	pid_t cpid;

        pid = getpid();

	shmid = shmget(IPC_PRIVATE, 2*sizeof(int), 0777|IPC_CREAT);
	if (fork() == 0)
	{

		b = (int *) shmat(shmid, 0, 0);
		for( i=0; i< 10; i++)
		{
			sleep(1);
		}
		shmdt(b);
	}
	else
	{
		a = (int *) shmat(shmid, 0, 0);

		a[0] = 0; a[1] = 1;
		for( i=0; i< 10; i++) {
			sleep(1);
			a[0] = a[0] + a[1];
			a[1] = a[0] + a[1];
		}
		wait(&status);
		shmdt(a);
		shmctl(shmid, IPC_RMID, 0);
	}

	printf("Enter the length of array: ");
        scanf("%d",&n);
        num = (int*)malloc(sizeof(int)*n);
        printf("Enter the array:\n");
        for(i = 0 ; i < n ; i++)
                scanf("%d",&num[i]);
        p = fork();
	
        if(p == 0)
        {
                printf("Parallel Processes Executing\n");
                for(i = 0 ; i < n ; i++ )
                {
                        for(j = 0 ; j < n-i-1 ; j++)
                        {	int temp;
                                if(num[j] > num[j+1])
                                {
                                        temp = num[j];
                                        num[j] = num[j+1];
                                        num[j+1] = temp;
                                }
                        }
                }
                printf("The sorted array is:\n");
                for(i = 0 ; i < n ; i++)
                {
                        printf("%d ",num[i]);
                }
                printf("\n");
        }
return 0;
}
