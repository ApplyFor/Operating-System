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
	int x;	//row
	int y;	//column
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
void* grid_check(void* data)
{
	parameters* r=(parameters* )data;
	int row=r->x;
	int column=r->y;
	int i,j;
	for(i=row;i<row+3;i++)
	{
		for(j=column;j<column+3;j++)
		{
			r->thread_number+=puzzle[i][j];
		}
	}
	sleep(4);
	pthread_exit(NULL);
}
void* row_check(void* data)
{
	parameters* r=(parameters* )data;
	int row=r->x;
	int column=r->y;
	int sum=0,tmp=0;
	int i,j;
	for(i=row;i<PUZZLE_SIZE+1;i++)
	{
		if(!rv)
			break;
		for(j=column;j<PUZZLE_SIZE+1;j++)
		{
			tmp+=puzzle[i][j];
		}
		if(!sum)
		{
			sum=tmp;
		}
		if(sum!=tmp)
		{
			rv=0;
			break;
		}
		tmp=0;
	}
	sleep(4);
	pthread_exit(NULL);
}
void* column_check(void* data)
{
	parameters* r=(parameters* )data;
	int row=r->x;
	int column=r->y;
	int sum=0,tmp=0;
	int i,j;
	for(i=column;i<PUZZLE_SIZE+1;i++)
	{
		if(!rv)
			break;
		for(j=row;j<PUZZLE_SIZE+1;j++)
		{
			tmp+=puzzle[j][i];
		}
		if(!sum)
		{
			sum=tmp;
		}
		else if(sum!=tmp)
		{
			rv=0;
			break;
		}
		tmp=0;
	}
	sleep(4);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int i;
    // input the sudoku file
    SetPuzzle("test.txt");
    
    parameters p[NUMBER_OF_THREADS];
    for(i=0;i<NUMBER_OF_THREADS;i++)
    {
    	p[i].thread_number=0;
		if(i==9 || i==10)
		{
			p[i].x=1;
			p[i].y=1;
		}
		else
		{
			if(i%3==0)
				p[i].x=1;
			else if(i%3==1)
				p[i].x=4;
			else
				p[i].x=7;
			
			if(i<3)
				p[i].y=1;
			else if(i<6)
				p[i].y=4;
			else
				p[i].y=7;
		} 
	} 
    
    pthread_t t[NUMBER_OF_THREADS];
    for(i=0;i<NUMBER_OF_THREADS;i++)
    {
    	if(i==9)
    		pthread_create(&t[i],NULL,row_check,(void*) &p[i]);
    	else if(i==10)
    		pthread_create(&t[i],NULL,column_check,(void*) &p[i]);
    	else
			pthread_create(&t[i],NULL,grid_check,(void*) &p[i]);
	}
    for(i=0;i<NUMBER_OF_THREADS;i++)
    	pthread_join(t[i],NULL);
    if(rv)
    {
    	int tmp=p[0].thread_number;
    	for(i=1;i<9;i++)
    	{
    		if(tmp!=p[i].thread_number)
    		{
    			rv=0;
    			break;
			}
		}
	}
	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}
