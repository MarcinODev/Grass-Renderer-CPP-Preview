#pragma once 
#include <vector>
#include <functional>
#include "engine.h"

/// <summary>
/// Task queue for executing objects after time
/// </summary>
class TaskTimer
{
	struct Task
	{
		float timeToTrigger;
		std::function<void(void)> onTrigger;
	};

public:
	TaskTimer(Engine* engine);
	~TaskTimer();

	/// <summary>
	/// Queues task
	/// </summary>
	/// <param name="delay">Delay in executing function. If value is == 0 than task will be executed in next frame</param>
	/// <param name="onTrigger">Function to call</param>
	void queueTask(float delay, std::function<void(void)> onTrigger);

	/// <summary>
	/// Refreshes tasks and calls the needed ones
	/// </summary>
	void update();

private:
	Engine* engine;
	std::vector<Task> tasks;
	std::vector<std::function<void(void)>> instantTasks;//executed as soon as loop run
	std::vector<std::function<void(void)>> executedInstantTasks;
};

