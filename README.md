ClusterFreak
============
C++ Winforms Spreadsheet program from CptS 223 - Data Structures.

Description: This program reads from an input text file, the name of which is read in from the keyboard. The first line of the input file will designate how many CPUs are in the cluster, while the rest of the lines designate jobs to be run on the cluster. The Job lines are of the format: <Job Name> <CPUs Required For Job> <Job Duration>. The program constructs Job objects based on this info and then adds these Jobs to a Cluster object simulating a cluster of CPUs. This program makes use of the Cluster class I developed, as well as the Job class used in the Cluster class.
