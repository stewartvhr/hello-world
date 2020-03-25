#include "Page.hpp"

#include <iostream>
#include <algorithm>

Page::Page() {}

Page::~Page() {}

// try insert one inputData to page
// return false if current page is full
bool Page::load(int inputData)
{
	bool loaded = true;
	if (int(dataVec.size()) == TUPLES_PER_PAGE) 
	{
		loaded = false;
	}
	else
	{
		dataVec.push_back(inputData);
	}
	return loaded;
}

// print out all data in the page
// starting with |, separated by space 
void Page::print() 
{
	std::cout << "| ";
	for (int data : dataVec) 
	{
		std::cout << data << " ";
	}
}

// return all data in the page 
std::vector<int> Page::getData()
{
	return dataVec;
}

// sort all data in the page
void Page::sortPage()
{
	std::sort(dataVec.begin(), dataVec.end());
}