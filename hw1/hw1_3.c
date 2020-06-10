#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void)
{	
	pid_t pid;
	printf("Main process id : %d.\n",getpid());
	
	pid = fork();//fork1
	if(pid==0){
		printf("Fork1, I'm the child %d, my parent is %d.\n",getpid(),getppid());
	}
	else{
		wait(NULL);
	}
	
	if(pid==0){
		pid=fork();//fork2
		if(pid==0){
			printf("Fork2, I'm the child %d, my parent is %d.\n",getpid(),getppid());
		}
		else{
			wait(NULL);
		}
	}
	pid=fork();//fork3
	if(pid==0){
		printf("Fork3, I'm the child %d, my parent is %d.\n",getpid(),getppid());
	}
	else{
		wait(NULL);
	}
	return 0;
} 
