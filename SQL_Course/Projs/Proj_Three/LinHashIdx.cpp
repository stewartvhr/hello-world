#include "LinHashIdx.hpp"

#include <functional>
#include <math.h>
#include <cassert>
#include <bitset>
#include <iostream>

// You should not need to modify this function.
// 
// If USING_HASH is true, then hash the input using std::hash.
// 
// Otherwise, directly convert the given string to an integer.
// If conversion fails, then prints an error message and throws an exception
static int custom_hash (std::string input)
{
    if (USING_HASH)
    {
        return std::hash<std::string>{}(input);
    }
    else
    {
        int value = 0;

        try
        {
            value = std::stoi(input);
        }
        catch (std::invalid_argument)
        {
            std::cerr << "Error: [" << input << "] could not be converted to an integer" << std::endl;
            throw;
        }
        catch (std::out_of_range)
        {
            std::cerr << "Error: [" << input << "] is out of range for the integer type" << std::endl;
            throw;
        }

        if (std::to_string(value) != input)
        {
            std::cerr << "Error: [" << input << "] could not be converted to an integer" << std::endl;
            throw std::invalid_argument("string2int(\"" + input + "\") failed");
        }

        return value;
    }
}

LinHashIdx::LinHashIdx() : next(0), level(0), num_curr_level_buckets(INITIAL_NUM_BUCKETS)
{
    // Check that INITIAL_NUM_BUCKETS is a postive power of 2
    assert(INITIAL_NUM_BUCKETS && (INITIAL_NUM_BUCKETS & (INITIAL_NUM_BUCKETS - 1)) == 0);
    // Check that MAX_BUCKET_SIZE is nonzero
    assert(MAX_BUCKET_SIZE);

    // Create all initial buckets
    directory.resize(INITIAL_NUM_BUCKETS);
    for (auto& bucketPtr : directory)
    {
        bucketPtr = new Bucket;
    }
}

// Insert the input into the correct bucket, and split the "next" bucket if the insert causes an overflow
// Only insert if the input does not already exist in the index

/**
 1. If index contains input, return
 2. Else, find bucket number
    a. If hi(key) < Next, return hi+1(key)
 3. Insert input into bucket if possible
 4. If not possible, check if bucket next
 5. If bucket next:
    a. split bucket w/ hi+1(key)
    b. create overlap on original bucket if needed
    c. next++
 6. If bucket not next:
    a. insert into overlap
    b. split next bucket w/ hi+1(key)
    c. next++
 7. if next == number of buckets at this level:
    a. next = 0
    b. level += 1
 */

void LinHashIdx::insert(std::string input)
{
    unsigned int bucket_num = -1;
    
    int int_input = custom_hash(input);
    
    if(!contains(input))
    {
        bucket_num = get_bucket_num(int_input);
        
        if(directory[bucket_num] -> is_bucket_full())
        {
            directory[bucket_num] -> insert(input);
            split_bucket(next);
            
            if(next == num_curr_level_buckets - 1)
            {
                start_next_round();
            } else
            {
                ++next;
            }
            /**if(bucket_num != next)
            {
                directory[bucket_num] -> insert(input);
                split_bucket(next);
                
                if(next == num_curr_level_buckets - 1)
                {
                    start_next_round();
                } else
                {
                    ++next;
                }
            } else
            {
                split_bucket(next);
                int new_bucket_num = hash(int_input, level + 1);
                directory[new_bucket_num] -> insert(input);
                
                if(next == num_curr_level_buckets - 1)
                {
                    start_next_round();
                } else
                {
                    ++next;
                }
            }*/
        } else
        {
            directory[bucket_num] -> insert(input);
        }
    }
}

void LinHashIdx::start_next_round()
{
    next = 0;
    level += 1;
    num_curr_level_buckets *=2;
}

void LinHashIdx::split_bucket(int bucket_num)
{
    directory.push_back(new Bucket);
    
    divide_store_keys(bucket_num);
}

void LinHashIdx::divide_store_keys(int bucket_num)
{
    Bucket * new_bucket_ptr = new Bucket;
    
    iterate_over_bucket(directory[bucket_num], new_bucket_ptr, bucket_num);
    
    delete directory[bucket_num];
    directory[bucket_num] = nullptr;
    
    directory[bucket_num] = new_bucket_ptr;
}

void LinHashIdx::iterate_over_bucket(Bucket * curr_bucket_ptr, Bucket * new_bucket_ptr,
                                     int bucket_num)
{
    std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> bucket_keys;
    
    prioritize_keys(curr_bucket_ptr, &bucket_keys);
    
    divide_keys(new_bucket_ptr, bucket_keys, bucket_num);
}

void LinHashIdx::prioritize_keys(Bucket * curr_bucket_ptr,
                     std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> * bucket_keys)
{
    std::vector<std::string>::const_iterator keys_const_begin = curr_bucket_ptr -> get_keys_const_begin();
    
    std::vector<std::string>::const_iterator keys_const_end = curr_bucket_ptr -> get_keys_const_end();
    
    while (keys_const_begin != keys_const_end)
    {
        bucket_keys->push(*keys_const_begin);
        ++keys_const_begin;
    }
    
    if(curr_bucket_ptr -> overflow_exists())
    {
        curr_bucket_ptr = curr_bucket_ptr -> get_overflow();
        prioritize_keys(curr_bucket_ptr, bucket_keys);
    }
}

void LinHashIdx::divide_keys(Bucket * new_bucket_ptr,
                 std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> bucket_keys,
                 int bucket_num)
{
    int curr_hash_value, curr_int_input = -1;
    
    while (!bucket_keys.empty())
    {
        std::string curr_key = bucket_keys.top();
       
        curr_int_input = custom_hash(curr_key);
        
        curr_hash_value = hash(curr_int_input, level + 1);
        
        if(curr_hash_value == bucket_num)
        {
            new_bucket_ptr -> insert(curr_key);
        } else
        {
            directory[directory.size() - 1] -> insert(curr_key);
        }
        bucket_keys.pop();
    }
}

int LinHashIdx::get_bucket_num(int int_input)
{
    int bucket_num = -1;

    unsigned int curr_level_hash = hash(int_input, level);
    
    if (curr_level_hash < next)
    {
        bucket_num = hash(int_input, level + 1);
    } else
    {
        bucket_num = curr_level_hash;
    }
    
    return bucket_num;
}

int LinHashIdx::hash(int int_input, unsigned int level)
{
    return (int)((int)int_input % (int)(pow(2.0, (double)level) * INITIAL_NUM_BUCKETS));
}

// Search the index for the given value, and return true if it's found
bool LinHashIdx::contains(std::string input)
{
    bool input_found = false;
    
    int int_input = custom_hash(input);
    
    int bucket_num = get_bucket_num(int_input);
    
    Bucket * curr_bucket_ptr = nullptr;
    
    curr_bucket_ptr = directory[bucket_num];
    
    input_found = contains_search(input, curr_bucket_ptr);
    
    return input_found;
}

bool LinHashIdx::contains_search(std::string input, Bucket * curr_bucket_ptr)
{
    bool input_found = false;
    
    std::vector<std::string>::const_iterator keys_const_begin = curr_bucket_ptr -> get_keys_const_begin();
    
    std::vector<std::string>::const_iterator keys_const_end = curr_bucket_ptr -> get_keys_const_end();
    
    while ((keys_const_begin != keys_const_end) and !input_found)
    {
        input_found = (*keys_const_begin == input);
        ++keys_const_begin;
    }
    
    if (input_found)
    {
        return input_found;
    } else if(curr_bucket_ptr -> overflow_exists())
    {
        curr_bucket_ptr = curr_bucket_ptr -> get_overflow();
        return contains_search(input, curr_bucket_ptr);
    } else
    {
        return input_found;
    }
    return input_found;
}

// DO NOT MODIFY THIS FUNCTION!
//
// Print the entire index.
// Prints the current level, each directory entry, and the keys in each bucket.
// Also points to the "next" bucket
void LinHashIdx::print()
{
    // The number of characters used to display the binary value of each bucket index
    size_t levelSize = level + log2(INITIAL_NUM_BUCKETS) + 1;
    std::string blankSpace(levelSize, ' ');

    std::cout << "Level: " << level << std::endl;

    for (unsigned int i = 0; i < directory.size(); ++i)
    {
        // The "next" bucket gets a pointer
        std::string nextStr(4, ' ');
        if (i == next)
        {
            nextStr = "--> ";
        }

        // The high-level representation of the bucket index uses 1 more bit than the low level representation
        std::bitset<BITSET_LEN> bitHash = i;
        std::string highLevelStr, lowLevelStr;

        highLevelStr = bitHash.to_string().substr(BITSET_LEN - levelSize, levelSize);
        highLevelStr += " ";
        lowLevelStr = highLevelStr.substr(1, levelSize);

        // If it's the beginning of a level, only the low level bucket index is displayed
        if (next == 0)
        {
            highLevelStr = "";
        }

        // Buckets above the initial set do not have a low level representation
        if (i >= INITIAL_NUM_BUCKETS * pow(2, level))
        {
            lowLevelStr = blankSpace;
        }

        std::cout << nextStr << highLevelStr << lowLevelStr;
        directory[i]->print();
        std::cout << std::endl;
    }

    std::cout << std::endl;
}
