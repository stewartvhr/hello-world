#include "Bucket.hpp"

#include <iostream>
#include <algorithm>

Bucket::Bucket() : overflowBucket(nullptr)
{

}

Bucket::~Bucket()
{
    if (overflowBucket)
    {
        delete overflowBucket;
    }
}

// Insert the key into the bucket, or one of its overflow buckets.
// Return true if an overflow bucket was used
bool Bucket::insert(std::string input)
{
    bool overflow_bucket_used = false;
    
    if((unsigned int)keys.size() < MAX_BUCKET_SIZE)
    {
        keys.push_back(input);
    } else
    {
        if (!overflowBucket) {
            overflowBucket = new Bucket;
        }
        
        overflowBucket -> insert(input);
        overflow_bucket_used = true;
    }

    return overflow_bucket_used;
}

bool Bucket::is_bucket_full()
{
    if ((unsigned int)keys.size() < MAX_BUCKET_SIZE) {
        return false;
    } else
    {
        return true;
    }
}

// DO NOT MODIFY THIS FUNCTION!
// 
// Print all keys in this bucket and all of its overflow buckets.
// Keys are separated by spaces, and buckets are separated by '|'.
void Bucket::print()
{
    std::cout << "| ";
    for (auto& key : keys)
    {
        std::cout << key << " ";
    }
    std::cout << "|";

    if (overflowBucket)
    {
        overflowBucket->print();
    }
}

std::vector<std::string>::iterator Bucket::get_keys_begin()
{
    return keys.begin();
}

std::vector<std::string>::iterator Bucket::get_keys_end()
{
    return keys.end();
}

std::vector<std::string>::const_iterator Bucket::get_keys_const_begin()
{
    return keys.begin();
}

std::vector<std::string>::const_iterator Bucket::get_keys_const_end()
{
    return keys.end();
}

Bucket* Bucket::get_overflow()
{
    return overflowBucket;
}

bool Bucket::overflow_exists()
{
    return((overflowBucket) ? true: false);
}

unsigned int Bucket::get_num_keys()
{
    return (unsigned int)keys.size();
}
