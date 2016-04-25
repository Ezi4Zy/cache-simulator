//
// Created by xueminsun on 16-4-25.
//
#include "trace_file.h"
#include "base.h"
#include "cache.h"

#include <iostream>

using namespace base;

int cache_size[] = {1024, 4*1024, 64* 1024, 128*1024};
int block_size[] = {8, 16, 32, 128};
MappingType type[] = {MT_DM, MT_2W, MT_4W, MT_FA};
WritePolicy policy[] = {WP_WB, WP_WT};


int main() {
	trace_file file("test2.trace");
	file.read_file();
	for(int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				for (int l = 0; l < 2; ++l) {
					cache c(cache_size[i], block_size[j], type[k], policy[l]);
					while (1) {
						static int index = 0;
						string line = file.getline();
						c.process_access(line);

						if (line == "")
							break;
					}
					std::cout << result2string(c.generate_result()) << std::endl;
					file.reset_index();
				}
}

