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
 * File Description: This file contains the Cluster class, which simulates a cluster of CPUs containing jobs waiting to run and jobs currently running.
					 This class depends on my Job class, and Adam Carter's StringSplitter class. This file was developed solely by me - Jesse Chisholm.
 */

#pragma once

#include <queue>
#include <iostream>

#include "Job.h"
#include "StringSplitter.h"

using namespace std;

//enables us to sort a priority_queue of Job elements by earliest end tick
struct running_comparison
{
	//overload "()", which is called by a priority_queue when making a heap
	bool operator () (const Job &a, const Job &b) const
	{
		//return whether a's end tick is later than b's end tick
		return a.getEndTick() > b.getEndTick();
	}
};

//enables us to sort a priority_queue of Job elements by shortest duration
struct waiting_comparison
{
	//overload "()", which is called by a priority_queue when making a heap
	bool operator () (const Job &a, const Job &b) const
	{
		//return whether a's duration is greater than b's duration
		//return a.getDuration() > b.getDuration();

		return a.getRequiredCPUs() > b.getRequiredCPUs();
	}
};

class Cluster
{
private:
	//basic info about cluster - self explanatory
	int _total_cpus = -1;
	int _free_cpus = -1;
	int _current_tick = 1;
	
	//lines from file about jobs
	queue<string> _infile_text;

	//lines going to output CSV file
	queue<string> _outfile_text;

	//priority queue [by shortest duration] of jobs waiting to be run
	priority_queue<Job, vector<Job>, waiting_comparison> _waiting_jobs;

	//priority queue [by earliest end tick] of running jobs
	priority_queue<Job, vector<Job>, running_comparison> _running_jobs;

public:
#pragma region Constructors

	//Default Constructor - set outfile text
	Cluster()
	{
		_outfile_text.push("Job Name,Starting Tick,Ending Tick,Duration");
	}

	//Constructor - info passed in: total cpus in cluster
	Cluster(int total_cpus)
	{
		_outfile_text.push("Job Name,Starting Tick,Ending Tick,Duration");
		setTotalCPUs(total_cpus);
		setFreeCPUs(total_cpus);
	}

#pragma endregion

#pragma region Getters and Setters

	int getTotalCPUs() const
	{
		return _total_cpus;
	}

	void setTotalCPUs(int total_cpus)
	{
		_total_cpus = total_cpus;
	}

	int getFreeCPUs() const
	{
		return _free_cpus;
	}

	void setFreeCPUs(int free_cpus)
	{
		_free_cpus = free_cpus;
	}

	int getCurrentTick() const
	{
		return _current_tick;
	}

	void setInfileText(queue<string> &infile_text)
	{
		_infile_text = infile_text;
	}

	queue<string> &getOutfileText()
	{
		return _outfile_text;
	}

#pragma endregion

#pragma region Priority Queue Methods

	//add a new job to the waiting job priority queue
	void addWaitingJob(Job &job)
	{
		_waiting_jobs.push(job);
	}

	//get the highest priority job in the waiting job queue
	Job getWaitingJob() const
	{
		return _waiting_jobs.top();
	}

	//pops the highest priority job in the waiting job queue
	void popWaitingJob()
	{
		_waiting_jobs.pop();
	}

	//gets the number of jobs in the waiting job queue
	int getNumWaiting() const
	{
		return _waiting_jobs.size();
	}

	//add a job to the running job priority queue
	void addRunningJob(Job &job)
	{
		_running_jobs.push(job);
	}

	//get the highest priority job in the running job queue
	Job getRunningJob() const
	{
		return _running_jobs.top();
	}

	//pops the highest priority job in the running job queue
	void popRunningJob()
	{
		_running_jobs.pop();
	}

	//gets the number of jobs in the running job queue
	int getNumRunning() const
	{
		return _running_jobs.size();
	}

#pragma endregion

#pragma region Other Methods

	//returns whether the cluster is done processing and running jobs
	bool isFinished()
	{
		return _waiting_jobs.empty() && _running_jobs.empty() && _infile_text.empty();
	}

	//tick time, perform main operations on cluster
	void tick()
	{
		cout << endl << endl << "*** Tick " << getCurrentTick() << " ***"
			<< endl << "Processing jobs file..." << endl;

		if (_infile_text.size() == 0)
		{
			cout << "There are no more jobs in the jobs file.";
		}
		else
		{
			//get next job description and pop file text queue
			string text = _infile_text.front();
			_infile_text.pop();

			if (text == "NULL")
			{
				cout << "NULL job encountered. Adding no jobs this tick.";
			}
			else
			{
				//split text by " "
				vector<string> fields = StringSplitter::split(text, " ");

				//construct a job from split text
				Job curr_job(fields[0], stoi(fields[1]), 
					stoi(fields[2]), getCurrentTick());
				
				cout << "The next job in the list is: " << curr_job.getName() << "." << endl
					<< "Adding job to jobs queue...";

				//push current job onto queue
				addWaitingJob(curr_job);
			}
		}

		cout << endl << endl << "Processing jobs queue..."
			<< endl << "Current number of scheduled jobs: "
			<< getNumWaiting() << "." << endl;
		
		//whether we're done adding jobs
		bool done = false;

		while (!done)
		{
			//if no jobs are waiting to be run
			if (getNumWaiting() == 0)
			{
				cout << "Jobs queue is empty.";

				//we are done adding waiting jobs, break loop
				done = true;
				break;
			}

			//get the highest priority job waiting to be run
			Job waiting_job = getWaitingJob();

			//cpus required by job in question
			int required_cpus = waiting_job.getRequiredCPUs();

			//free CPUs in the cluster
			int free_cpus = getFreeCPUs();

			//if the cluster can accomodate the waiting job's CPU needs
			if (free_cpus >= required_cpus)
			{
				cout << "Adding the highest priority job:" << endl
					<< waiting_job.getName() << "\t"
					<< waiting_job.getRequiredCPUs() << "\t"
					<< waiting_job.getDuration() << endl;


				//set cluster's free CPUs
				setFreeCPUs(free_cpus - required_cpus);

				//end tick is current tick + duration of job
				waiting_job.setEndTick(getCurrentTick() + waiting_job.getDuration() - 1);

				//add job to running jobs and pop the waiting job
				addRunningJob(waiting_job);
				popWaitingJob();
			}
			else
			{
				cout << "Not enough free CPUs (requested: " << required_cpus << ", "
					<< "actual: " << free_cpus << "). Waiting for resources...";

				//done adding waiting jobs
				done = true;
			}
		}

		cout << endl << endl << "Running active jobs..." << endl;
		done = false;

		while (!done)
		{
			//if no jobs are waiting to be run
			if (getNumRunning() == 0)
			{
				//we are done adding running jobs, break loop
				done = true;
				break;
			}

			//get the running job with lowest end tick 
			Job curr_job = getRunningJob();

			//get the end tick of the current job
			int end_tick = curr_job.getEndTick();

			//if job has ended
			if (end_tick <= getCurrentTick())
			{
				cout << "Job \"" << curr_job.getName()
					<< "\" is complete. Freeing up resources." << endl;

				//job data
				string name = curr_job.getName();
				int start = curr_job.getStartTick();
				int end = curr_job.getEndTick();

				//output CSV data
				string output = "\"" + name + "\"" + ","
					+ to_string(start) + ","
					+ to_string(end) + ","
					+ to_string(end - start);

				//add output text to outfile text
				_outfile_text.push(output);
				
				//free up CPUs the current job had been using and pop running job
				setFreeCPUs(getFreeCPUs() + curr_job.getRequiredCPUs());
				popRunningJob();
			}
			else
			{
				//no jobs are done running, so we're done checking
				done = true;
			}
		}

		//advance tick count
		_current_tick++;
	}

#pragma endregion

};