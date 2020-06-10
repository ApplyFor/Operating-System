#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

typedef struct process
{
	int arrival;
	int burst;
	int waiting;
	int turnaround;
	int index;
}Process;

bool asort(Process a,Process b)
{
	return a.arrival<b.arrival;
}
bool bsort(Process a,Process b)
{
	return a.burst<b.burst;
}

int total;
vector<Process> p;

void read(string filename)
{
	ifstream file;
	file.open(filename);
	int flag=0;
    if(!file)
    {
    	cout<<"ERROR!"<<endl;
	}
	else
    {
    	file>>total;
    	p.resize(total);
		for(int i=0;i<total;i++)
			file>>p[i].arrival;
		for(int i=0;i<total;i++)
			file>>p[i].burst;
		
		for(int i=0;i<total;i++)
		{
			p[i].waiting=0;
			p[i].turnaround=p[i].burst;
			p[i].index=i;
		}
		
		sort(p.begin(),p.end(),asort);
		
		file.close();
	}
	
    return;
}

void value(vector <Process> x,int n)
{
	cout<<"total:"<<total<<endl;
	cout<<"arrival time:";
	for(int i=0;i<n;i++)
		cout<<x[i].arrival<<" ";
	cout<<endl;
	cout<<"burst time:";
	for(int i=0;i<n;i++)
	{
		cout<<x[i].burst<<" ";
	}
	cout<<endl;
	cout<<"index:";
	for(int i=0;i<n;i++)
	{
		cout<<x[i].index<<" ";
	}
	cout<<endl<<endl;
	
	return;
}

int main(int argc, char* argv[])
{
	read(argv[1]);
	//read("Q1.txt");
	//value(p,total);
	
	vector <Process> queue;
	int num=0,empty,sim;
	for(int time=0;;time++)
	{
		empty=1;
		sim=0;
		if(num==total && queue.empty())
			break;
		if(!queue.empty())
			empty=0;
		while(time==p[num].arrival && num!=total)
		{
			queue.push_back(p[num]);
			num+=1;
			if(!sim)
				sim=-1;
			else if(sim=-1)
				sim=1;
		}
		if(empty && sim)
		{
			sort(queue.begin(),queue.end(),bsort);
		}
		else
		{
			sort(queue.begin()+1,queue.end(),bsort);
		}
		if(!queue.empty())
			queue[0].burst-=1;
		if(!queue[0].burst)
		{
			p[queue[0].index].waiting=time+1-p[queue[0].index].burst-p[queue[0].index].arrival;
			p[queue[0].index].turnaround+=p[queue[0].index].waiting;
			queue.erase(queue.begin()); 
		} 
		//cout<<"time:"<<time<<endl;
		//value(queue,queue.size());
	}
	double avgw=0;
	double avgta=0;
	ofstream file;
	file.open("ans1.txt");
	if(!file)
    {
    	cout<<"ERROR!"<<endl;
	}
	else
    {
    	for(int i=0;i<total;i++)
    	{
    		avgw+=p[i].waiting;
    		avgta+=p[i].turnaround;
    		file<<p[i].waiting<<" "<<p[i].turnaround<<endl;
		}
		avgw/=total;
		avgta/=total;

		if(avgw==int(avgw))
			file<<int(avgw)<<endl;
		else
		{
			int tmp=avgw*100000,time=5;
			//cout<<tmp<<endl;
			while(tmp%10==0)
			{
				tmp/=10;
				time-=1;
			}
			file<<fixed<<setprecision(time)<<avgw<<endl;
		}
		if(avgta==int(avgta))
			file<<int(avgta)<<endl;
		else
		{
			int tmp=avgta*100000,time=5;
			//cout<<tmp<<endl;
			while(tmp%10==0)
			{
				tmp/=10;
				time-=1;
			}
			file<<fixed<<setprecision(time)<<avgta<<endl;
		}
	}
	file.close();
	
	return 0;
} 
