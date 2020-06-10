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

int total,quantum1,quantum2;
vector<Process> p;

void read(string filename)
{
	ifstream file;
	file.open(filename);
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
		file>>quantum1>>quantum2;
		
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
	cout<<endl;
	
	return;
}

int main(int argc, char* argv[])
{
	read(argv[1]);
	//read("Q4.txt");
	//value(p,total);
	
	vector <Process> queue1,queue2,queue3;
	int num=0,run1=quantum1,run2=quantum2,minus;
	for(int time=0;;time++)
	{
		minus=0;
		
		if(num==total && queue1.empty()  && queue2.empty()  && queue3.empty() )
			break;
		while(time==p[num].arrival && num!=total)
		{
			queue1.push_back(p[num]);
			num+=1;
		}
		
		if(!queue1.empty())
		{	
			if(!run1)
			{
				run1=quantum1;
				queue2.push_back(queue1[0]);
				queue1.erase(queue1.begin());
			}	
			if(!queue1.empty())
			{
				queue1[0].burst-=1;
				run1-=1;
				minus=1;
				if(!queue1[0].burst)
				{
					run1=quantum1;
					p[queue1[0].index].waiting=time+1-p[queue1[0].index].burst-p[queue1[0].index].arrival;
					queue1.erase(queue1.begin()); 
				}
			}
		}
		if(queue1.empty() && !queue2.empty())
		{
			if(!run2)
			{
				run2=quantum2;
				queue3.push_back(queue2[0]);
				queue2.erase(queue2.begin());
				sort(queue3.begin(),queue3.end(),bsort);
			} 
			if(!queue2.empty())
			{
				if(!minus)
				{
					queue2[0].burst-=1;
					run2-=1;
					minus=1;
				}
				if(!queue2[0].burst)
				{
					run2=quantum2;
					p[queue2[0].index].waiting=time+1-p[queue2[0].index].burst-p[queue2[0].index].arrival;
					queue2.erase(queue2.begin()); 
				}
			}
		}
		if(queue1.empty() && queue2.empty() && !queue3.empty())
		{
			if(!minus)
			{
				queue3[0].burst-=1;
			}
			if(!queue3[0].burst)
			{
				p[queue3[0].index].waiting=time+1-p[queue3[0].index].burst-p[queue3[0].index].arrival;
				queue3.erase(queue3.begin());
			} 
		}
		
		/*cout<<"time: "<<time+1<<endl;
		cout<<"run: "<<run1<<" "<<run2<<endl;
		if(!queue1.empty())
		{
			cout<<"queue1"<<endl<<"=================================================="<<endl;
			value(queue1,queue1.size());
		}
		if(!queue2.empty())
		{
			cout<<"queue2"<<endl<<"=================================================="<<endl;
			value(queue2,queue2.size());
		}
		if(!queue3.empty())
		{
			cout<<"queue3"<<endl<<"=================================================="<<endl;
			value(queue3,queue3.size());
		}
		cout<<endl;*/ 
	}
	for(int i=0;i<total;i++)
	{
		p[i].turnaround+=p[i].waiting;
	}
	
	double avgw=0;
	double avgta=0;
	ofstream file;
	file.open("ans4.txt");
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
