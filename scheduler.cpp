#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include "scheduler.h"

using namespace std;

PrepareSchedule::PrepareSchedule(vector<SchedulerTask*> t, std::chrono::system_clock::time_point start){
	enabledTasks = t;
	startTime = start;
}

void PrepareSchedule::calculateCycleTime(vector<SchedulerTask*> sorted){
	cycleTime = sorted[0]->getPeriod();
	enabledTasks = sorted;
}

vector<SchedulerTask*> PrepareSchedule::RecalculateActiveTasks(std::chrono::system_clock::time_point time_of_last_cycle){
	vector<SchedulerTask*> activeTasks;
	std::chrono::milliseconds wait_for_cycle = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_of_last_cycle);
	while(wait_for_cycle < cycleTime){
		wait_for_cycle = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_of_last_cycle);
		;
	}
	std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < enabledTasks.size(); ++i)
	{
		if((enabledTasks[i]->getPeriod().count())*(enabledTasks[i]->getCallNum()) < std::chrono::duration_cast<std::chrono::milliseconds>(current_time - startTime + cycleTime).count()) 
		{
			activeTasks.push_back(enabledTasks[i]);
		}
	}
	return activeTasks;
}

void PrepareSchedule::setStartTime(std::chrono::system_clock::time_point newStart){
	startTime = newStart;
}

void PrepareSchedule::resetAllTaskCalls(){
	for (unsigned int i = 0; i < enabledTasks.size(); ++i)
	{
		enabledTasks[i]->resetCallNum();
	}
}

vector<SchedulerTask*> getSchedule(vector<SchedulerTask*> unorderedTasks){
	sort(unorderedTasks.begin(), unorderedTasks.end(), sortByPeriod);
	unsigned int i = 0;
	while (i < unorderedTasks.size())
	{
		int j=1;
		while(i+j<unorderedTasks.size() && unorderedTasks[i]->getPeriod() == unorderedTasks[i+j]->getPeriod()){
			j++;
		}
		if(j>1){
			sort(unorderedTasks.begin()+i, unorderedTasks.begin()+i+j-1, sortByPriority);
		}
		i+=j;
	}
	return unorderedTasks;
}

bool sortByPeriod (SchedulerTask *lhs, SchedulerTask *rhs) { return lhs->getPeriod() < rhs->getPeriod(); }
bool sortByPriority (SchedulerTask *lhs, SchedulerTask *rhs) { return lhs->getPriority() < rhs->getPriority(); }
