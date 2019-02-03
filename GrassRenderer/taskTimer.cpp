#include "taskTimer.h"



TaskTimer::TaskTimer(Engine * engine)
{
	this->engine = engine;
}

TaskTimer::~TaskTimer()
{
}

void TaskTimer::queueTask(float delay, std::function<void(void)> onTrigger)
{
	if (delay > 0)
	{
		float timeToTrigger = delay + engine->getTime();
		int i = 0;
		for (; i < tasks.size(); i++)
		{
			if (tasks[i].timeToTrigger > timeToTrigger)
			{
				break;
			}
		}

		tasks.insert(tasks.begin() + i, Task{ timeToTrigger, onTrigger });
	}
	else
	{
		instantTasks.push_back(onTrigger);
	}
}

void TaskTimer::update()
{
	float time = engine->getTime();
	int i = 0;
	for (; i < tasks.size(); i++)
	{
		if (tasks[i].timeToTrigger <= time)
		{
			tasks[i].onTrigger();
		}
		else
		{
			break;
		}
	}

	if (i > 0)
	{
		tasks.erase(tasks.begin(), tasks.begin() + i);
	}

	std::swap(executedInstantTasks, instantTasks);//in case when instant task queues new instant task
	for (i = 0; i < executedInstantTasks.size(); i++)
	{
		executedInstantTasks[i]();
	}

	executedInstantTasks.clear();
}
