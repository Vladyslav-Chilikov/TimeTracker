#include "Logging.h"


bool Logging::parseData()	// takes commit data and divide it on 8 parts
{
	bool result = 1;
	std::string separator = ";";
	std::string tempData = this->data;
	int sepResult;
	for (int i = 0; i < 8; i++)
	{
		sepResult = tempData.find(separator);
		if (sepResult == -1 && i != 7)	// basicaly means invalid data on input
		{
			result = 0; break;
		}
		else
		{
			this->logData[i] = tempData.substr(0, tempData.find(separator));
			tempData.erase(0, tempData.find(separator) + separator.length());
		}
	}
	return result;
}

bool Logging::verifyData()	// particial data validation
{
	bool result = 1;
	result = parseData();
	int time = stoi(this->logData[7]);
	std::string date = this->logData[6];
	std::string email = this->logData[1];
	if (time <= 0 || time >= 24) { result = 0; }	// invalid time condition
	if (email.find('@') == -1) { result = 0; }	// invalid email condition
	std::string dateSeparator = "-";
	int sepResult;
	int dateInfo[3] = {0,0,0};
	for (int i = 0; i < 3; i++)	// date info division
	{	
		sepResult = date.find(dateSeparator);
		if (sepResult == -1 && i != 2)
		{
			result = 0; break;
		}
		else
		{
			dateInfo[i] = stoi(date.substr(0, date.find(dateSeparator)));
			date.erase(0, date.find(dateSeparator) + dateSeparator.length());
		}
	}
	if(dateInfo[0] <= 999 || dateInfo[0] >= 10000 || dateInfo[1] <= 0 || dateInfo[1] >= 13 || dateInfo[2] <= 0 || dateInfo[2] >= 32) { result = 0; }	// invalid date condition
	return result;
}

void Logging::printData()	// print commit data (if it is valid)
{
	for (int i = 0; i < 8; i++)
	{
		std::cout << "logData " << i+1 << " - " << this->logData[i] <<std::endl;
	}
}

void Logging::Init()	// files and streams initialization
{
	this->tTrack.open("TData.CSV", std::ios::out | std::ios::in | std::ios::app);
	// if file does not exist create file
	if (!this->tTrack.is_open())
	{
		this->tTrack.close();
		this->tTrack.open("TData.CSV", std::ios::out);
		this->tTrack.close();
		this->tTrack.open("TData.CSV", std::ios::out | std::ios::in | std::ios::app);
	}
	// if file is empty add column names
	bool empty = (this->tTrack.peek() == EOF);
	this->tTrack.clear();
	if (empty) 
	{ 
		this->tTrack << "Name;Email;Department;Position;Project;Task;Date;Hours;" << std::endl; 
	}

	this->tReport.open("TSummary.CSV", std::ios::out | std::ios::in | std::ios::app);
	// if file does not exist create file
	if (!this->tReport.is_open())
	{
		this->tReport.close();
		this->tReport.open("TSummary.CSV", std::ios::out);
		this->tReport.close();
		this->tReport.open("TSummary.CSV", std::ios::out | std::ios::in | std::ios::app);
	}
	// if file is empty add column names
	empty = (this->tReport.peek() == EOF);
	this->tReport.clear();
	if (empty)
	{
		this->tReport << "Name;Month;Total hours;" << std::endl;
	}

}

Logging::Logging()	// constructor
{
	this->Init();
}

void Logging::WriteLine()	// adds new commit to TData.CSV
{
	if (this->verifyData())
	{
		if (!this->tTrack.is_open())
		{
			std::cout << "Something is wrong with Time Data file" << std::endl;
		}
		else
		{
			std::vector<std::string> existingData = ReadFile(this->tTrack);
			this->tTrack.close();
			this->tTrack.open("TempTData.CSV", std::ios::out);
			this->tTrack.close();
			this->tTrack.open("TempTData.CSV", std::ios::out | std::ios::in | std::ios::app);
			existingData.push_back(this->data);
			for (int i = 0; i < existingData.size(); i++)
			{
				this->tTrack << existingData[i];
				this->tTrack << std::endl;
			}
			this->tTrack.close();
			remove("TData.CSV");
			(void)rename("TempTData.CSV", "TData.CSV");
			this->tTrack.open("TData.CSV", std::ios::out | std::ios::in | std::ios::app);
			std::cout << "Time Data file was updated successfuly" << std::endl;
			recalculateReport();
		}
	}
	else
	{
		std::cout << "Wrong Data on Input" << std::endl;
	}
}

std::vector<std::string> Logging::ReadFile(std::fstream &fst)	// read axisting data from CSV file
{
	std::vector<std::string> rows;
	int rowsCounter = 0;
	std::string line;
	fst.seekg(0);
	while (getline(fst, line))
	{
		rowsCounter++;
		rows.push_back(line);
		line.clear();
	}
	return rows;
}

void Logging::recalculateReport()	// updates existing data or adds new data using commit information
{
	std::string month[] = { "January", "February", "March", "April", "May", "June",
							 "July", "August", "September", "October", "November", "December" };

	if (!this->tReport.is_open())
	{
		std::cout << "Something is wrong with Time Summary file" << std::endl;
	}
	else
	{
		std::vector<std::string> existingData = ReadFile(this->tReport);
		std::string report;
		std::string separator = ";";
		std::string dateSeparator = "-";
		std::string date = this->logData[6];
		int dateInfo[2] = { 0,0 };
		for (int i = 0; i < 2; i++)
		{
				dateInfo[i] = stoi(date.substr(0, date.find(dateSeparator)));
				date.erase(0, date.find(dateSeparator) + dateSeparator.length());
			
		}
		std::string monthAndYear = month[dateInfo[1]] + ' ' + std::to_string(dateInfo[0]);
		std::vector<std::string> TempExistingData = existingData;
		for (int i = 0; i < TempExistingData.size(); i++)	//if Summary file already contains row with similar data update it
		{
			std::string tempName = TempExistingData[i].substr(0, TempExistingData[i].find(separator));
			if (tempName == this->logData[0])
			{
				TempExistingData[i].erase(0, TempExistingData[i].find(separator) + separator.length());
				std::string tempMonthAndYear = TempExistingData[i].substr(0, TempExistingData[i].find(separator));
				if (tempMonthAndYear == monthAndYear)
				{
					TempExistingData[i].erase(0, TempExistingData[i].find(separator) + separator.length());
					int TempHours = stoi(TempExistingData[i]) + stoi(this->logData[7]);
					std::cout << existingData[i] << " was updated to --> ";
					existingData[i] = existingData[i].erase(existingData[i].find_last_of(separator)+1, existingData[i].size()) + std::to_string(TempHours);
					std::cout << existingData[i] << std::endl;
					break;
				}
			}
			if (i == TempExistingData.size() - 1) 
			{ 
				report = this->logData[0] + ';' + monthAndYear + ';' + this->logData[7];
				existingData.push_back(report); 
			}
		}
		this->tReport.close();
		this->tReport.open("TempTSummary.CSV", std::ios::out);
		this->tReport.close();
		this->tReport.open("TempTSummary.CSV", std::ios::out | std::ios::in | std::ios::app);
		for (int i = 0; i < existingData.size(); i++)
		{
			this->tReport << existingData[i];
			this->tReport << std::endl;
		}
		this->tReport.close();
		remove("TSummary.CSV");
		(void)rename("TempTSummary.CSV", "TSummary.CSV");
		this->tReport.open("TSummary.CSV", std::ios::out | std::ios::in | std::ios::app);
		std::cout << "Time Summary file was updated successfuly" << std::endl;
	}
}