#include <iostream>
#include "vector.h"

int main()
{
    custom::Vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    custom::Vector<int> numbers2 = numbers;

    for(int i = 0; i < numbers.size(); i++)
    {
        std::cout << numbers[i] << " ";
        //std::cout << numbers.at(i) << " ";
    }

    return 0;
}