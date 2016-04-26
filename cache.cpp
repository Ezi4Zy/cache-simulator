//
// Created by xueminsun on 16-4-25.
//
#include <vector>
#include <string>
#include "cache.h"
#include "trace_file.h"

using base::result;
using std::vector;
using std::string;

typedef struct memory_access {
    bool read_flag;
    unsigned int address;
} memory_access;

static unsigned int hex2int(const string& hex) {
	unsigned int value = 0;
	sscanf(hex.c_str(), "%x", &value);
	return value;
}

memory_access cache::str2ma(string str) {
	memory_access ma;
	ma.read_flag = (str.find("read") != string::npos);
	size_t address = str.find("0x");
	ma.address = hex2int(str.substr(address, 10));
	return ma;
}

cache::cache(int cache_size, int block_size, base::MappingType type, base::WritePolicy policy) {
	this->cache_size = cache_size;
	this->block_size = block_size;
	this->mapping_type = type;
	this->write_back = policy;
	this->miss_counter = 0;
	this->hit_counter = 0;
	this->memory2cache = 0;
	this->cache2memory = 0;
	this->cmp_count = this->block_numbers_row = type != base::MT_FA ? type : cache_size / block_size;
	this->datas.resize(cache_size / block_size);
}

cache::~cache() {
	this->datas.clear();
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
					this->memory2cache += block_size;
				}
			} else {
				if (process_write_through(ma.address)) {
					this->hit_counter++;
				} else {
					this->miss_counter++;
					this->memory2cache += block_size;
				}
				this->cache2memory += 4;
			}
		}
	} else {
		if (this->write_back) {                       //process access is finished, cal the cache2memory in write_back policy
			for (int i = 0; i < datas.size(); ++i) {
				if (datas[i].is_dirty)
					this->cache2memory += block_size; //transfer dirty data from cache to memory
			}
		}
	}
}

bool cache::process(unsigned int address, bool is_write) {
	int rows = this->cache_size / (this->block_size * this->block_numbers_row);
	int row = (address / block_size) % rows;
	int begin = row * block_numbers_row;
	int max_index = begin;
	int max_count = -1;
	address = address / block_size ;
	for (int i = 0; i < this->block_numbers_row; ++i) {
		if (!datas[begin + i].is_used) {      //miss, write in unused slot
			datas[begin + i].is_used = true;
			datas[begin + i].is_dirty = is_write;
			datas[begin + i].address = address;
			datas[begin + i].LRU_counter = 0;
			return false;
		} else {
			if (address == datas[begin + i].address) {          //hit
				datas[begin + i].LRU_counter = 0;
				datas[begin + i].is_dirty |= is_write;
				i++;
				while (i < this->block_numbers_row && datas[begin + i].is_used) {
					datas[begin + i].LRU_counter++;
					i++;
				}
				return true;
			} else {
				if (datas[begin + i].LRU_counter > max_count) {
					max_count = datas[begin + i].LRU_counter;
					max_index = begin + i;
				}
				datas[begin + i].LRU_counter++;
			}
		}
	}
	if (datas[max_index].is_dirty && this->write_back) {  //transfer dirty data from cache to memory
		this->cache2memory += this->block_size;
	}
	datas[max_index].LRU_counter = 0; //miss and no unused slot, replace the one which LRU_counter is max
	datas[max_index].address = address;
	datas[max_index].is_dirty = is_write;
	return false;
}

bool cache::process_read(unsigned int address) {
	return process(address, false);
}

bool cache::process_write_back(unsigned int address) {
	return process(address, true);
}

bool cache::process_write_through(unsigned int address) {
	return process(address, true);
}

result cache::generate_result() const {
	result r;
	r.cache_size = this->cache_size;
	r.block_size = this->block_size;
	r.mapping_type = this->mapping_type;
	r.write_policy = this->write_back ? base::WP_WB : base::WP_WT;
	r.hit_rate = double(this->hit_counter) / (this->hit_counter + this->miss_counter);
	r.mem2cache = this->memory2cache;
	r.cache2mem = this->cache2memory;
	r.cmp_count = this->cmp_count;

	return r;
}
