#include "ExtSortRange.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>

// initializa Chunk of Disk memory, each page in a group by itself
// create first empty page
ExtSortRange::ExtSortRange() : groupSize(1)
{
	Page* firstPage = new Page;
	pageRange.push_back(firstPage);
}

// delete all page from pageRange
ExtSortRange::~ExtSortRange() 
{
	while (!pageRange.empty()) {
		delete pageRange.back();
		pageRange.pop_back();
	}
}

// given unsorted input values, load them into pages
// create new page when necessary, and bookmark by pageRange vector
void ExtSortRange::load(std::string inputStr) 
{
	std::stringstream iss(inputStr);
	int input;
	while (iss >> input)
	{
		if (!pageRange.back()->load(input)) {
			Page* newPage = new Page;
			newPage->load(input);
			pageRange.push_back(newPage);
		}
	}
}

void ExtSortRange::print()
{
	for (auto page : pageRange) 
	{
		page->print();
	}
	std::cout << " |" << std::endl;
}

void ExtSortRange::extMergeSort()
{
	pass0Sort();
	std::cout << "-->pass 0" << std::endl;
	print();

	int passNum = 1;
	while (passMergeSort()) 
	{
		std::cout << "-->pass " << passNum++ << std::endl;
		print();
	}
	groupSize = 1; // reset groupSize for further sort
}


// TODO: implement pass 0 of external merge sort
// Hint: You may use method functions of Page class
void ExtSortRange::pass0Sort() 
{
	runs.clear();
	num_vals = 0;
	//number of pages * num_tuples per page 
	//num_vals = (pageRange.size())*(PAGE_SIZE / TUPLE_SIZE);
	//Sort data on each page individually
	for(int i = 0; i < (int)pageRange.size(); i++) {
		num_vals += pageRange[i]->getData().size();
		pageRange[i]->sortPage();
		runs.push_back(pageRange[i]->getData());
	}

	return;
}

// TODO: implement one merge sort pass (not pass0) of external merge sort
// return false if no need to sort (actually do nothing)
// Hint: You may use member variable
bool ExtSortRange::passMergeSort() 
{
	//int n_pages = pageRange.size();
	//check if sorted, return false if already sorted
	if((int)runs[0].size() == num_vals) {
		runs.clear();

		return false;
	}

	std::vector<int> run_vals;
	std::vector<std::vector<int>> final_runs;


	//Watch for out of bounds
	for(int i = 0; i < ((int)runs.size()); i+=0) {

		//Get B-1 Runs, stop if reaching out of bounds
		for(int k = 0; ((i) < (int)runs.size()) && (k < (BUFFER_PAGE - 1)); k++) {
			for(int j = 0; j < (int)runs[i].size(); j++) {
				run_vals.push_back(runs[i][j]);
			}
			i = i + 1;

		}
		//Sort B-1 Runs
		std::sort(run_vals.begin(), run_vals.end());
		groupSize += (int)run_vals.size()/(PAGE_SIZE / TUPLE_SIZE);
		//Store run
		final_runs.push_back(run_vals);
		//Clear out previous runs to set up new B-1 Runs
		run_vals.clear();

	}

	int page_count = 0;
	Page* newPage = new Page;

	runs.clear();
	//Rewrite page ranges
	for(int i = 0; i < (int)final_runs.size(); ++i) {
		for(int j = 0; j < (int)final_runs[i].size(); ++j) {
			//If page is full
			if(!newPage->load(final_runs[i][j])) {
				//Delete old page in page range
				delete pageRange[page_count];
				//Update page in page range
				pageRange[page_count] = newPage;
				page_count++;
				//Create new page 
				newPage = new Page;
				//Add new value to new page
				newPage->load(final_runs[i][j]);
			}
			//std::cout << final_runs[i][j] << " ";

		}
		//update runs
		runs.push_back(final_runs[i]);
	}
	delete pageRange[page_count];
	//Update page in page range
	pageRange[page_count] = newPage;


	return true;	
}