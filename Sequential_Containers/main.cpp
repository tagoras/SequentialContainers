#include <iostream>
#include "Vector.h"
#include <string>

// Static Linkage of Boost.test
#define BOOST_TEST_MODULE Vector_Testing
#include <boost/test/unit_test.hpp>

custom::Vector<int> test_vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

BOOST_AUTO_TEST_CASE(test_vector_default_constructor)
{
	custom::Vector<int> vec{};
	BOOST_TEST(vec.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_constructor)
{
	custom::Vector<int> vec{ 1,2,3,4,5,6,7,8,9,10 };
	BOOST_TEST(vec.size() == 10);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
	custom::Vector<int> vec{ 1,2,3,4,5,6,7,8,9,10 };
	vec.pop_back();
	BOOST_TEST(vec.size() == 9);
}

BOOST_AUTO_TEST_CASE(test_insert_single_element)
{
	custom::Vector<std::string> vec{"One", "Two", "Three", "Four", "Five"};
	BOOST_TEST(vec.size() == 5);
	vec.erase("Five");
	BOOST_TEST(vec.size() == 4);
	std::cout << vec << std::endl;
}
