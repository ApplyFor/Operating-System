#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void)
{
	char *arg[MAX_LINE/2+1]; /*command line arguments*/
	int should_run = 1; /*flag to determine when to exit program*/
	
	char inputBuffer[MAX_LINE];
	int len;
	int i;
	int index,pos;
	int should_wait; //flag to determine whether to invoke wait()
	
	while(should_run){
		printf("osh>");
		fflush(stdout);
		
		/**
		* your code!
		* After reading user input, the step are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will not invoke wait()
		*/
		

		memset(inputBuffer,0,MAX_LINE);
		should_wait=1;
		index=0;
		pos=-1;
		len=read(STDIN_FILENO, inputBuffer, MAX_LINE);

		if(inputBuffer[0]=='e' && inputBuffer[1]=='x' && inputBuffer[2]=='i' && inputBuffer[3]=='t' && inputBuffer[4]=='\n')	//exit
		{
			should_run=0;
			continue;
		}
		for(i=0;i<len;i++)
		{
			if(inputBuffer[i]=='\n')
			{
				if(pos!=-1)
				{
					arg[index]=&inputBuffer[pos];
					index+=1; 
				}
				pos=-1;
				inputBuffer[i]='\0';
				arg[index]=NULL;
			}
			else if(inputBuffer[i]=='&')
			{
				inputBuffer[i]='\0';
				should_wait=0;
			}
			else if(inputBuffer[i]==' ' || inputBuffer[i]=='\t')
			{
				if(pos!=-1)
				{
					arg[index]=&inputBuffer[pos];
					index+=1; 
				}
				pos=-1;
				inputBuffer[i]='\0';
			}
			else
			{
				if(pos==-1)
				{
					pos=i;
				}
			}
		}
		
		pid_t pid;
		pid=fork();
		//printf("pid:%d\n",pid);
		//printf("pid:%d ppid:%d\n",getpid(),getppid());
		if(pid==0)
			execvp(arg[0],arg);
		if(should_wait==1)
			waitpid(pid,NULL,0);
	}

	return 0;
}

