#include <iostream>
#include "vector.h"
#include <utility> // for std::move

int main()
{
    custom::Vector<int> ivec;

    int *p1 = nullptr, *p2 = nullptr;

    for(int i = 0; i < 100; i++)
    {
        ivec.push_back(i);
    }

    for(int &element : ivec)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    std::cout << ivec[10] << std::endl;

    return 0;
}