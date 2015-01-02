/*
 * Programmer: Jesse Chisholm
 * Program: Job Program (HW6)
 * Date Started: 4/8/14
 * Date Completed: 4/14/14
 * Hours Worked: 20+
 * Collaboration: Breifly bounced some ideas off of Ben Windishar-Tatham.
 *
 * Program Description: This program reads from an input text file, the name of which is read in from the keyboard. The first line of the input file
 *						will designate how many CPUs are in the cluster, while the rest of the lines designate jobs to be run on the cluster. The
 *						Job lines are of the format: <Job Name> <CPUs Required For Job> <Job Duration>. The program constructs Job objects based on
 *						this info and then adds these Jobs to a Cluster object simulating a cluster of CPUs. This program makes use of the Cluster
 *						class I developed, as well as the Job class used in the Cluster class.
 *
 * File Description: This file contains the Job class, which simulates a single job on a cluster of CPUs. This class is used by my Cluster class.
 *					 This file was developed solely by me - Jesse Chisholm.
 */

#pragma once

#include <string>

using namespace std;

class Job
{
private:
	//basic info about job - self explanatory
	string _name = "Unknown";
	int _required_cpus = -1;
	int _duration = -1;
	int _start_tick = -1;
	int _end_tick = -1;

public:
#pragma region Constructors

	//Default Constructor - empty
	Job()
	{

	}

	//Constructor - info passed in: name, required cpus, duration of job, current tick
	Job(string name, int required_cpus, int duration, int current_tick)
	{
		//set name, required CPUs and duration of job normally
		setName(name);
		setRequiredCPUs(required_cpus);
		setDuration(duration);

		//set start tick to the current tick
		setStartTick(current_tick);
	}

#pragma endregion

#pragma region Getters and Setters

	string getName() const
	{
		return _name;
	}

	void setName(string name)
	{
		if (name != "")
		{
			_name = name;
		}
	}

	int getRequiredCPUs() const
	{
		return _required_cpus;
	}

	void setRequiredCPUs(int required_cpus)
	{
		if (required_cpus > 0)
		{
			_required_cpus = required_cpus;
		}
	}

	int getDuration() const
	{
		return _duration;
	}

	void setDuration(int duration)
	{
		if (duration > 0)
		{
			_duration = duration;
		}
	}

	int getStartTick() const
	{
		return _start_tick;
	}

	void setStartTick(int start_tick)
	{
		if (start_tick > 0)
		{
			_start_tick = start_tick;
		}
	}

	int getEndTick() const
	{
		return _end_tick;
	}

	void setEndTick(int end_tick)
	{
		if (end_tick > 0)
		{
			_end_tick = end_tick;
		}
	}

#pragma endregion

};