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

    std::cout << ivec << std::endl;

    return 0;
}