#ifndef _LINHASHIDX_HPP_
#define _LINHASHIDX_HPP_

#include <vector>
#include <queue>

#include "Bucket.hpp"
#include "constants.hpp"

class LinHashIdx
{
public:

    LinHashIdx();
    void insert(std::string input);
    bool contains(std::string input);
    bool contains_search(std::string input, Bucket * curr_bucket_ptr);
    int get_bucket_num(int int_input);
    int hash(int int_input, unsigned int level);
    void split_bucket(int bucket_num);
    void divide_store_keys(int bucket_num);
    void iterate_over_bucket(Bucket * curr_bucket_ptr, Bucket * new_bucket_ptr,
                             int bucket_num);
    void prioritize_keys(Bucket * curr_bucket_ptr,
                         std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> * bucket_keys);
    void divide_keys(Bucket * new_bucket_ptr,
                     std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> bucket_keys,
                     int bucket_num);
    void start_next_round();
    void print();

private:

    std::vector<Bucket*> directory;
    unsigned int next;
    unsigned int level;
    unsigned int num_curr_level_buckets;

};

#endif
