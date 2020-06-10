#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

int rv = 1; // flag to check answer
/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0}
		};

/* data structure for passing data to threads */
typedef struct
{
	int thread_number;
	int x;
	int y;
} parameters;

/* print puzzle */ 
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
	        printf("|%1d |",grid[i][j]);
		}
        printf(COLLINE);
	}
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;            
            } 
        }
    }
    print_grid(puzzle);
    return;
}
void* sum_check(void* arg)
{
	int sum=0,tmp=0;
	int i,j,m,n;
	for(i=1;i<=9;i++)		//row
	{
		if(!rv)
			break;
		for(j=1;j<=9;j++)
		{
			tmp+=puzzle[i][j];
		}
		if(!sum)
		{
			sum=tmp;
			tmp=0;
		}
		else if(sum==tmp)
		{
			tmp=0;
			continue;
		}
		else
		{
			rv=0;
			break;
		}
	}
	for(i=1;i<=9;i++)		//column
	{
		if(!rv)
			break;
		for(j=1;j<=9;j++)
		{
			tmp+=puzzle[j][i];
		}
		if(!sum)
		{
			sum=tmp;
			tmp=0;
		}
		else if(sum==tmp)
		{
			tmp=0;
			continue;
		}
		else
		{
			rv=0;
			break;
		}
	}
	for(m=0;m<3;m++)		//grid
	{
		if(!rv)
			break;
		for(n=0;n<3;n++)
		{
			if(!rv)
				break;
			for(i=3*m+1;i<=3*m+3;i++)
			{
				for(j=3*n+1;j<=3*n+3;j++)
				{
					tmp+=puzzle[i][j];
				}
			}
			if(!sum)
			{
				sum=tmp;
				tmp=0;
			}
			else if(sum==tmp)
			{
				tmp=0;
				continue;
			}
			else
			{
				rv=0;
				break;
			}
		}
	}
	sleep(4);
	pthread_exit(NULL);
	
}
int main(int argc, char* argv[])
{
    // input the sudoku file
    SetPuzzle("test.txt");
    
    pthread_t t;
    pthread_create(&t,NULL,sum_check,NULL);
    pthread_join(t,NULL);
    
	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}
