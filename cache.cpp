//
// Created by xueminsun on 16-4-25.
//
#include <vector>
#include <string>
#include "cache.h"
#include "base.h"

using namespace base;
using std::vector;
using std::string;

typedef struct memory_access {
    bool read_flag; //0:read 1:write
    unsigned int address;
} memory_access;

long long cache::total_address = 4294967296;    //cache_datas total count

memory_access cache::str2ma(string str) {
	memory_access ma;
	ma.read_flag = (str.find("read") != string::npos);
	size_t address = str.find("0x");
	ma.address = hex2int(str.substr(address, 10).c_str());
	return ma;
}

cache::cache(int cache_size, int block_size, MappingType type, WritePolicy policy) {
	this->cache_size = cache_size;
	this->block_size = block_size;
	this->mapping_type = type;
	this->write_back = policy;
	this->miss_counter = 0;
	this->hit_counter = 0;
	this->memory2cache = 0;
	this->cache2memory = 0;
	this->cmp_count = this->block_numbers_row = type != MT_FA ? type : cache_size / block_size;
	this->cache_datas.resize(cache_size / block_size);
}

cache::~cache() {
	this->cache_datas.clear();
}

void cache::process_access(string str) {
	if (str != "") {
		memory_access ma = str2ma(str);
		if (ma.read_flag) {
			if (process_read(ma.address)) {
				hit_counter++;
			} else {
				miss_counter++;
				this->memory2cache += this->block_size;
			}
		} else {
			if (this->write_back) {
				if (process_write_back(ma.address)) {
					this->hit_counter++;
				} else {
					this->miss_counter++;
				}
			} else {
				if (process_write_throught(ma.address)) {
					this->hit_counter++;
					this->cache2memory += 4;
				} else {
					this->miss_counter++;
					this->cache2memory += 4;
				}
			}
			this->memory2cache += this->block_size;
		}
	} else {
		if (this->write_back) {                       //process access is finished, cal the cache2memory in write back
			for (int i = 0; i < cache_datas.size(); ++i) {
				if (cache_datas[i].is_dirty)
					this->cache2memory += this->block_size; //transfer dirty data from cache to memory
			}
		}
	}
}

bool cache::process(unsigned int address, bool is_write) {
	int rows = this->cache_size / (this->block_size * this->block_numbers_row);
	long long address_range = total_address / rows;
	int row = address / address_range;
	int begin = row * this->block_numbers_row;
	int max_index = begin;
	int max_count = -1;
	for (int i = 0; i < this->block_numbers_row; ++i) {
		if (!cache_datas[begin + i].is_used) {      //search all and no hit, but has unused slot
			cache_datas[begin + i].is_used = true;
			cache_datas[begin + i].is_dirty = is_write;
			cache_datas[begin + i].address = address;
			cache_datas[begin + i].LRU_counter = 0;
			return false;
		} else {
			int diff = address - cache_datas[begin + i].address;
			if (diff < this->block_size && diff > 0) {          //hit
				cache_datas[begin + i].LRU_counter = 0;
				cache_datas[begin + i].is_dirty = is_write;
				i++;
				while (i < this->block_numbers_row && cache_datas[begin + i].is_used) {
					cache_datas[begin + i].LRU_counter++;
					i++;
				}
				return true;
			} else {
				if (cache_datas[begin + i].LRU_counter > max_count) {
					max_count = cache_datas[begin + i].LRU_counter;
					max_index = begin + i;
				}
				cache_datas[begin + i].LRU_counter++;
			}
		}
	}
	if (cache_datas[max_index].is_dirty && this->write_back) {
		this->cache2memory += this->block_size;
	}
	cache_datas[max_index].LRU_counter = 0;
	cache_datas[max_index].address = address;
	cache_datas[max_index].is_dirty = is_write;
	return false;
}

bool cache::process_read(unsigned int address) {
	return process(address, false);
}

bool cache::process_write_back(unsigned int address) {
	return process(address, true);
}

bool cache::process_write_throught(unsigned int address) {
	return process(address, true);
}

result cache::generate_result() const {
	result r;
	r.cache_size = this->cache_size;
	r.block_size = this->block_size;
	r.mapping_type = this->mapping_type;
	r.write_policy = this->write_back ? WP_WB : WP_WT;
	r.hit_rate = this->hit_counter / (this->hit_counter + this->miss_counter);
	r.mem2cache = this->memory2cache;
	r.cache2mem = this->cache2memory;
	r.cmp_count = this->cmp_count;

	return r;
}
