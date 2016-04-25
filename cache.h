//
// Created by xueminsun on 16-4-25.
//

#ifndef CACHE_SIMULATOR_CACHE_H
#define CACHE_SIMULATOR_CACHE_H

#include "base.h"
#include <vector>
#include <string>
struct memory_access;
using namespace base;
typedef struct cache_data{
    bool is_used;
	bool is_dirty;
    unsigned int address;
	int LRU_counter;
    cache_data(){is_used = false; is_dirty = false; address = 0; LRU_counter = 0;}
}cache_data;
using std::vector;
using std::string;
class cache{
public:
	explicit cache(int cache_size, int block_size, MappingType type, WritePolicy policy);
	~cache();
	void process_access(string str);
	result generate_result() const ;
private:
	static memory_access str2ma(string str);
    bool process(unsigned int address, bool is_write);
    bool process_read(unsigned int address);
	bool process_write_back(unsigned int address);
    bool process_write_throught(unsigned int address);
private:
	static long long total_address;
	int cache_size;             //Cache size
	int block_size;             //Block size
	base::MappingType mapping_type; //Mapping type
	bool write_back;            //Write policy
	int miss_counter;            //memory miss count
	int hit_counter;            //memory hit count
	int cache2memory;           //Total bytes transferred from memory to the cache
	int memory2cache;           //Total bytes transferred from cache to memory
	int cmp_count;              //Number of comparisons to identify a hit
    int block_numbers_row;      // blocks number in a row
	vector<cache_data> cache_datas;
};

#endif //CACHE_SIMULATOR_CACHE_H
