// by Vladyslav Chilikov 
// 2022

#include <iostream>

#include "Logging.h"

/*
Done additional tasks:
	An example of data validation.
	Logging class with output to separate file.
	*Input loop for numerous inputs.
*/

int main()
{
	Logging log;
	bool bContinue = true;
	while (bContinue)
	{
		std::cout << "Please add new time period:" << std::endl;
		std::getline(std::cin, log.data);
		std::cout << std::endl;
		log.WriteLine();

		std::cout << "Do you want to add another time period?  1 - yes / 0 - no" << std::endl;
		std::cin >> bContinue;
		std::cin.ignore();
	}
	return 0;
}
