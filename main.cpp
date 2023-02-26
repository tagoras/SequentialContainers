#include <iostream>
#include "vector.h"
#include <utility> 


/*
	TODO:

	Add unit tests using boost.test
*/

int main()
{
	custom::Vector<int> numbers = { 1, 2, 3, 4, 5 };

	for (auto element : numbers)
	{
		std::cout << element << " ";
	}

	std::cout << "\n--------------------------" << std::endl;

	std::cout << numbers << std::endl;

	std::cout << "--------------------------" << std::endl;

	numbers.push_back({ 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 });

	std::cout << numbers << std::endl;

	
	/*
		Fix pop_back(), currently throws exception
	*/
	//numbers.pop_back();
	//numbers.pop_back();
	///numbers.pop_back();

	std::cout << "--------------------------" << std::endl;

	std::cout << numbers << std::endl;
}


