//
// Created by fear on 2016/4/24.
//

#ifndef CACHE_SIMULATOR_RESULT_H
#define CACHE_SIMULATOR_RESULT_H

#include <string>

using std::string;

namespace base {
    enum MappingType {
        MT_DM = 1,
        MT_2W = 2,
        MT_4W = 4,
        MT_FA = 8,
    };
    enum WritePolicy {
        WP_WT = 0,
        WP_WB = 1,
    };


    typedef struct _result {
        int cache_size;
        int block_size;
        MappingType mapping_type;
        WritePolicy write_policy;
        double hit_rate;
        int mem2cache;
        int cache2mem;
        int cmp_count;
    } result;

    /*
     * transfer result to string
     */
    string result2string(const result& r);

}

#endif //CACHE_SIMULATOR_RESULT_H
