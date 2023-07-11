#pragma once
#include<string>
#include <fstream>
#include <iostream>
#include <vector>

class Logging {
public:
	Logging();
	std::string data;	//contains commit info
	bool parseData();
	bool verifyData();
	void printData();
	void Init();
	void WriteLine();
	void recalculateReport();
	std::vector <std::string> ReadFile(std::fstream &fst);
private:
	std::string logData[8];	//contains validated commit data
	std::fstream tTrack, tReport;	//streams to work with Data and Summary files
};