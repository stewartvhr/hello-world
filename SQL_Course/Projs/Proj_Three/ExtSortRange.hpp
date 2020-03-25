#ifndef _EXTSORTRANGE_HPP_
#define _EXTSORTRANGE_HPP_

#include <vector>

#include "Page.hpp"
#include "constants.hpp"

class ExtSortRange
{
public:

	ExtSortRange();
	~ExtSortRange();
	void load(std::string inputStr);
	void print();
	void extMergeSort();

private:
	
	std::vector<Page *> pageRange;
	int groupSize;
	void pass0Sort();
	bool passMergeSort();
	const int BUFFER_PAGE = MEM_BUFFER_SIZE / PAGE_SIZE; //TODO: assign correct value

	std::vector<std::vector<int>> runs;
	int num_vals;

};

#endif
