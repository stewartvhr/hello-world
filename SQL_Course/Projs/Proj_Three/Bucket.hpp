#ifndef _BUCKET_HPP_
#define _BUCKET_HPP_

#include <vector>
#include <string>

#include "constants.hpp"

class Bucket
{
public:

    Bucket();
    ~Bucket();
    bool insert(std::string input);
    bool is_bucket_full();
    void print();
    std::vector<std::string>::iterator get_keys_begin();
    std::vector<std::string>::iterator get_keys_end();
    std::vector<std::string>::const_iterator get_keys_const_begin();
    std::vector<std::string>::const_iterator get_keys_const_end();
    Bucket* get_overflow();
    bool overflow_exists();
    unsigned int get_num_keys();

private:

    std::vector<std::string> keys;
    Bucket* overflowBucket;

};

#endif
