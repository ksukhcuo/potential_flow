//
//  GnuplotInterface.h
//

#ifndef ___Gnuplot__
#define ___Gnuplot__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <unistd.h>

class Gnuplot{
private:
	double mWaitTime; // parameter controlling animation speed : default 0.1
	std::string mPath; // full path to gnuplot executable : default [/opt/local/bin/gnuplot]
	FILE* mFile; // file pointer for piping your application with gnuplot (legacy C-style code)
	const double mSecond = 0.1; // system constant
	void DefaultsInConstructor();
public:
	// constructor and destructor //
	Gnuplot(); // invoke child process for gnuplot using popen function
	Gnuplot(std::string path);
	~Gnuplot(); // terminate current pipe
	// public functions to access private data members //
	void SetWaitTime(double waitTime); // set wait time
	double GetWaitTime(); // return mWaitTime
	std::string SetPath(std::string path); // set full path for gnuplot executable
	std::string GetPath(); // return mPath;
	std::string SetGraphRange(std::string direction, double min, double max); // set plot range for x: 0, y: 1, z: 2
	std::string SetAxisLabel(std::string direction, std::string label); // set label to the axis specified with "direction"
	std::string SetAxisLog(std::string direction); // set log axis
	std::string SetAxisLog(std::string direction, double base); // set log axis with base
	std::string UnsetAxisLog(std::string direction); // unset log axis
	void Injection(std::string line); // inject arbitrary line into gnuplot
	void Injection(double x, double y); // data injection
	void Injection(double x, double y, double z); // data injection
	void Mesh(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> f); // plot mesh
	void Vector(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> u, std::vector<double> v); // plot vector
	void Vector(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> u, std::vector<double> v, double scale); // plot vector with scale
	void Flush(); // flush plot window
};

#endif /* defined(___gnuplotInterface__) */
