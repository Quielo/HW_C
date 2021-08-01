#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>

int main()
{
	int id;
	id = fork();
	if(id = 0)
	{
		printf("%s %d\n","Soy el hijo y mi id: ", getpid());
		sleep(10);
	}else{
		wait(NULL);
		printf("%s %d\n", "Soy el padre y mi id: ", getpid());
	}
}
