#include<list>
#include<queue>
#include<vector>

using namespace std;

typedef struct
{
	int arrive_time;	//the time customer arrive
	int duration;	//duration time 
}QNode;

typedef struct
{
	int occurtime;	//
	int type;	// 0: arriaving event; 1~n: n windows leaving
}Event;

typedef list<Event> EventList;
EventList eventlist;
Event event;
vector<list<QNode>> windowsQue;
int totaltime;
int custumernum;
int closetime = 1000;

void OpenForDay();
void arrivingFunc();
void leavingFun();

vector<pair<int, int>> record_event;
vector<pair<int, int>> record_gen;
int main()
{
	OpenForDay();
	while (!eventlist.empty())
	{
		event.occurtime = eventlist.front().occurtime;
		event.type = eventlist.front().type;
		record_event.push_back({ event.occurtime ,event.type });
		eventlist.pop_front();
		if (event.type == 0)
		{
			arrivingFunc();
		}
		else
		{
			leavingFun();
		}
	}
	int avg = totaltime / custumernum;
	return 0;
}


void orderInsert(EventList& li, const Event& en)
{
	if (li.empty())
	{
		li.insert(li.begin(), en);
		return;
	}

	bool insertflag = false;
	for (list<Event>::iterator it = li.begin(); it != li.end(); it++)
	{
		if (en.occurtime < it->occurtime)
		{
			li.insert(it, en);
			insertflag = true;
			break;
		}
	}
	if (!insertflag)
	{
		li.insert(li.end(), en);
	}
}

int miniumQueue(const vector<list<QNode>>& q)
{
	int cusnum = INT_MAX;
	int minqueue = -1;
	try
	{
		for (int j = 0; j < (int)q.size(); j++)
		{
			if (q[j].size() == 0)
			{
				return j;
			}
			if ((int)q[j].size() < cusnum)
			{
				cusnum = q[j].size();
				minqueue = j;
			}
		}
	}
	catch (const std::exception&)
	{
		return -1;
	}
	return minqueue;
}

void OpenForDay()
{
	int totaltime = 0;
	int closetime = 0;
	int custumernum = 0;
	event.occurtime = 0; event.type = 0;
	orderInsert(eventlist, event);
	windowsQue = vector<list<QNode>>(4);
}

void arrivingFunc()
{
	++custumernum;
	int intertime = rand() % 10;
	int duration = rand() % 30;
	record_gen.push_back({ intertime,duration });
	int t = event.occurtime + intertime;
	if (t > closetime)return;
	orderInsert(eventlist, { t, 0 });
	int i = miniumQueue(windowsQue);
	windowsQue[i].push_back({ t,duration });
	if (windowsQue[i].size() == 1)
	{
		orderInsert(eventlist, { t + duration, i+1 });
	}
}

void leavingFun()
{
	int i = event.type- 1;
	totaltime += event.occurtime - windowsQue[i].front().arrive_time;
	windowsQue[i].pop_front();
	if (!windowsQue[i].empty())
	{
		orderInsert(eventlist, { windowsQue[i].front().arrive_time + windowsQue[i].front().duration, i+1 });
	}
}