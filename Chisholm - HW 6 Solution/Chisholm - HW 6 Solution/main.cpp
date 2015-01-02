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
 * File Description: This file contains main, the driver of the program.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Cluster.h"
#include "StringSplitter.h"

using namespace std;

int main()
{

#pragma region Input File Work

	//name of input file
	string infile_name;

	//get file name from user
	cout << "Enter the name of your input file: ";
	getline(cin, infile_name);

	//open input file
	ifstream infile(infile_name);

	//line number in file
	int line_num = 1;
	
	//total CPUs in cluster
	int total_cpus;
	
	queue<string> infile_text;

	if (infile.is_open())
	{
		while (infile.good())
		{
			//line from file
			string line;

			//get line from file
			getline(infile, line);

			//most of the stuff after this point in the while loop is error checking
			//kind clunky, but gets the job done
			
			//heh, pun

			if (line_num == 1)
			{
				try
				{
					//set total_cpus from first line in file
					total_cpus = stoi(line);
				}
				catch (...)
				{
					//output error message
					cout << "Error: line 1 of input file invalid. Expected an integer number of CPUs. Exiting." << endl;
					return 0;
				}
			}
			else
			{
				//split line by " "
				vector<string> fields = StringSplitter::split(line, " ");

				if (fields.size() != 3)
				{
					if (fields[0] != "NULL")
					{
						//output error message
						cout << "Error: line " << line_num << " of input file invalid. Expected a job. Exiting." << endl;
						return 0;
					}
				}
				else
				{
					//get name from split line
					string name = fields[0];

					//individual job info
					int required_cpus, duration;

					try
					{
						required_cpus = stoi(fields[1]);
					}
					catch (...)
					{
						//output error message
						cout << "Error: line " << line_num << " of input file invalid. Expected a job. Exiting." << endl;
						return 0;
					}

					if (required_cpus > total_cpus)
					{
						//output error message
						cout << "Error: line " << line_num << " of input file invalid. Job requested too many CPUs. Exiting." << endl;
						return 0;
					}
					
					try
					{
						duration = stoi(fields[2]);
					}
					catch (...)
					{
						//output error message
						cout << "Error: line " << line_num << " of input file invalid. Expected a job. Exiting." << endl;
						return 0;
					}
				}
					
				//store line in file text queue
				infile_text.push(line);
			}

			line_num++;
		}
	}

	infile.close();

#pragma endregion

#pragma region Cluster Work
	
	//our cluster, initialized with designated total CPUs
	Cluster cluster(total_cpus);

	//set infile text for cluster from what we grabbed from the infile
	cluster.setInfileText(infile_text);

	//while the cluster is still running
	while (!cluster.isFinished())
	{
		//tick time for cluster, perform all major operations
		cluster.tick();
	}

#pragma endregion

#pragma region Output File Work

	//get outfile text from cluster
	queue<string> outfile_text = cluster.getOutfileText();

	//open output CSV file
	ofstream outfile("results.csv");

	//while outfile text is non-empty
	while (!outfile_text.empty())
	{
		//write line from outfile text to outfile
		outfile << outfile_text.front();

		//if not on last iteration
		if (outfile_text.size() != 1)
		{
			//output endline
			outfile << endl;
		}

		//pop outfile text queue
		outfile_text.pop();
	}

	outfile.close();

#pragma endregion

	return 0;
}