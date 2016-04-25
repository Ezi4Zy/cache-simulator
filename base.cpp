//
// Created by fear on 2016/4/24.
//
#include <stdio.h>
#include "base.h"

using namespace base;

string base::result2string(result &r) {
    string mapping_type;
    string write_policy;
    switch (r.mapping_type) {
        case MappingType::MT_DM:
            mapping_type += "DM";
            break;
        case MappingType::MT_2W:
            mapping_type += "2W";
            break;
        case MappingType::MT_4W:
            mapping_type += "4W";
            break;
        case MappingType::MT_FA:
            mapping_type += "FA";
            break;
        default:
            break;
    }
    switch (r.write_policy) {
        case WritePolicy::WP_WB:
            write_policy += "WB";
            break;
        case WritePolicy::WP_WT :
            write_policy += "WT";
            break;
        default:
            break;
    }
    //1024	8	DM	WB	0.16	216	96	1
    char temp[100];
    sprintf(temp, "%d\t%d\t%s\t%s\t%.2f\t%d\t%d\t%d", r.cache_size, r.block_size, mapping_type.c_str(),
            write_policy.c_str(), r.hit_rate, r.mem2cache,
            r.cache2mem, r.cmp_counter);
    return string(temp);
}

int base::hex2int(const char *hex) {
    int value = 0;
    sscanf(hex, "%x", &value);
    return value;
}


