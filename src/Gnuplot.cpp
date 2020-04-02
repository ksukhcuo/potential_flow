//
//  Gnuplot.cpp
//

#include "Gnuplot.h"

Gnuplot::Gnuplot(){
	mPath = "gnuplot"; // if gnuplot is added to PATH
	this->DefaultsInConstructor();
}

Gnuplot::Gnuplot(std::string path){
	mPath = path;
	this->DefaultsInConstructor();
}

void Gnuplot::DefaultsInConstructor(){
	mFile = popen(mPath.c_str(), "w"); // open pipe to gnuplot
	mWaitTime = 0.1*mSecond; // default waiting time for animation
	fprintf(mFile, "set palette rgb 33,13,10\n");
	fprintf(mFile, "set hidden3d\n");
	fprintf(mFile, "set ticslevel 0\n");
	fprintf(mFile, "set xlabel font 'Arial, 20'\n");
	fprintf(mFile, "set ylabel font 'Arial, 20'\n");
	fprintf(mFile, "set zlabel font 'Arial, 20'\n");
	fprintf(mFile, "set tics font 'Arial, 15'\n");
	fprintf(mFile, "set key font 'Arial, 15'\n");
}
Gnuplot::~Gnuplot(){
	pclose(mFile); // close pipe when an instance is terminated
}

std::string Gnuplot::SetPath(std::string path){
	mPath = path; // set path
	pclose(mFile); // close pipe
	this->DefaultsInConstructor();
	return "Path to your gnuplot is specified as " + path + "\n";
}

std::string Gnuplot::GetPath(){
	return mPath;
}

std::string Gnuplot::SetGraphRange(std::string direction, double min, double max){
	assert(mFile != nullptr);
	assert(direction == "x" || direction == "y" || direction == "z");
	std::string dir = " xr ";
	if(direction == "y") dir = " yr ";
	if(direction == "z") dir = " zr ";
	dir = "set" + dir + "[" + std::to_string(min) + ":" + std::to_string(max) + "]\n";
	fprintf(mFile, "%s", dir.c_str());
	return "In " + direction + " direction, plot range was set to [" + std::to_string(min) + ": " + std::to_string(max) + "]\n";
}

std::string Gnuplot::SetAxisLabel(std::string direction, std::string label){
	assert(mFile != nullptr);
	assert(direction == "x" || direction == "y" || direction == "z");
	std::string dir = " xlabel ";
	if(direction == "y") dir = " ylabel ";
	if(direction == "z") dir = " zlabel ";
	dir = "set" + dir + "'" + label + "'" + "\n";
	fprintf(mFile, "%s", dir.c_str());
	return "In " + direction + " direction, axis label was set to " + label + "\n";
}

std::string Gnuplot::SetAxisLog(std::string direction){
	return this->SetAxisLog(direction, 10.0);
}

std::string Gnuplot::SetAxisLog(std::string direction, double base){
	assert(mFile != nullptr);
	assert(direction == "x" || direction == "y" || direction == "z");
	std::string dir = " logscale " + direction + " " + std::to_string(base);
	dir = "set" + dir + "\n";
	fprintf(mFile, "%s", dir.c_str());
	return "In " + direction + " direction, axis was set as log axis\n";
}

std::string Gnuplot::UnsetAxisLog(std::string direction){
	assert(mFile != nullptr);
	std::string dir = " logscale " + direction;
	dir = "unset" + dir + "\n";
	fprintf(mFile, "%s", dir.c_str());
	return "In " + direction + " direction, log axis was unset\n";
}

void Gnuplot::Injection(std::string line){
	fprintf(mFile, "%s", line.c_str());
}

void Gnuplot::Injection(double x, double y){
	this->Injection(std::to_string(x) + ", " + std::to_string(y) + "\n");
}

void Gnuplot::Injection(double x, double y, double z){
	this->Injection(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "\n");
}

void Gnuplot::Mesh(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> f){
	assert(mFile != nullptr);
	int dataSizeX = (int)x.size();
	int dataSizeY = (int)y.size();
	assert(dataSizeX*dataSizeY == f.size());
	assert(f.size() > 0);
	std::string dgrid3d = "set dgrid3d " + std::to_string(dataSizeX) + ", " + std::to_string(dataSizeY) + "\n";
	fprintf(mFile, "%s", dgrid3d.c_str());
	qualifier = qualifier + " '-' using 1:2:3 with lines title 'Velocity potential'\n";
	fprintf(mFile, "%s", qualifier.c_str());
	for(int j=0; j<dataSizeY; j++){
		for(int i=0; i<dataSizeX; i++){
			int m = dataSizeX*j + i;
			std::string data = std::to_string(x[i]) + ", " + std::to_string(y[j]) + ", " + std::to_string(f[m]) + "\n";
			fprintf(mFile, "%s", data.c_str());
		}
	}
	fprintf(mFile, "e\n"); // e means the end of the data set, f(x, y)
}

void Gnuplot::Vector(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> u, std::vector<double> v){
	assert(mFile != nullptr);
	int dataSizeX = (int)x.size();
	int dataSizeY = (int)y.size();
	assert(dataSizeX*dataSizeY == u.size());
	assert(u.size() == v.size());
	qualifier = qualifier + " '-' using 1:2:3:4:(sqrt($3*$3+$4*$4)) with vector lc palette \n";
	fprintf(mFile, "%s", qualifier.c_str());
	for(int j=0; j<dataSizeY; j++){
		for(int i=0; i<dataSizeX; i++){
			int m = dataSizeX*j + i;
			std::string data = std::to_string(x[i]) + ", " + std::to_string(y[j]) + ", " + std::to_string(u[m]) + ", " + std::to_string(v[m]) + "\n";
			fprintf(mFile, "%s", data.c_str());
		}
	}
	fprintf(mFile, "e\n"); // e means the end of the data set, u(x, y)
}
void Gnuplot::Vector(std::string qualifier, std::vector<double> x, std::vector<double> y, std::vector<double> u, std::vector<double> v, double scale){
	assert(mFile != nullptr);
	int dataSizeX = (int)x.size();
	int dataSizeY = (int)y.size();
	assert(dataSizeX*dataSizeY == u.size());
	assert(u.size() == v.size());
	qualifier = qualifier + " '-' using 1:2:3:4:(sqrt($3*$3+$4*$4)) with vector lc palette \n";
	fprintf(mFile, "%s", qualifier.c_str());
	for(int j=0; j<dataSizeY; j++){
		for(int i=0; i<dataSizeX; i++){
			int m = dataSizeX*j + i;
			std::string data = std::to_string(x[i]) + ", " + std::to_string(y[j]) + ", " + std::to_string(u[m]*scale) + ", " + std::to_string(v[m]*scale) + "\n";
			fprintf(mFile, "%s", data.c_str());
		}
	}
	fprintf(mFile, "e\n"); // e means the end of the data set, u(x, y)
}

void Gnuplot::Flush(){
	assert(mFile != nullptr);
	fflush(mFile);
	sleep(mWaitTime);
}