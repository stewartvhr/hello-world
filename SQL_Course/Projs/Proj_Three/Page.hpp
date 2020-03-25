#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <vector>
#include <string>

#include "constants.hpp"

class Page
{
public:

	Page();
	~Page();
	bool load(int input);
	void print();
	void sortPage();
	std::vector<int> getData();

private:
	std::vector<int> dataVec; // each int value stands for one data tuple
	const int TUPLES_PER_PAGE = PAGE_SIZE / TUPLE_SIZE; // TODO: assign correct value
};

#endif
