//
// Created by fear on 2016/4/24.
//

#include "base.h"
#include <iostream>
using namespace base;
using std::cout;
using std::endl;

void test_result2string(){

    //1024	8	DM	WB	0.16	216	96	1
    result res;
    res.cache_size = 1024;
    res.block_size = 8;
    res.mapping_type = MappingType::MT_DM;
    res.write_policy = WritePolicy::WP_WT;
    res.hit_rate = 0.16;
    res.mem2cache = 216;
    res.cache2mem = 96;
    res.cmp_counter = 1;
    cout << result2string(res) << endl;
}


void test_hex2int(){
    char* hex = "0x02000006";
    cout << hex2int(hex) << endl;
}
int main(){
    test_result2string();
    test_hex2int();
}

