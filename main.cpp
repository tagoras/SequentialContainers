#include <iostream>
#include "vector.h"
#include <utility> // for std::move

int main()
{
    custom::Vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    custom::Vector<int> numbers2{std::move(numbers)};
    
    for(int element : numbers2)
    {
        std::cout << element << " ";
    }

    

    return 0;
}