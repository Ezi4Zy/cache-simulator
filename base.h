//
// Created by fear on 2016/4/24.
//

#ifndef CACHE_SIMULATOR_RESULT_H
#define CACHE_SIMULATOR_RESULT_H

#include <string>

using std::string;
namespace base {
    enum MappingType {
        MT_DM = 0,
        MT_2W,
        MT_4W,
        MT_FA,
    };
    enum WritePolicy {
        WP_WB = 0,
        WP_WT,
    };


    typedef struct _result {
        int cache_size;
        int block_size;
        MappingType mapping_type;
        WritePolicy write_policy;
        double hit_rate;
        int mem2cache;
        int cache2mem;
        int cmp_counter;   //验证是否命中cache的比较次数。
        // 映射方式为直接映射(MT_DM)，比较次数为1；映射方式为二路相联(MT_2W)，比较次数为2；
        // 映射方式为四路相联(MT_4W)，比较次数为4；映射方式为全相联(MT_FA)，比较次数为cache_size/block_size；
    } result;

    /*
     * 将result转为字符串输出
     */
    string result2string(result &r);

    /*
    * 功能：将十六进制字符串转换为整型(int)数值
    */
    int hex2int(const char *hex);


}

#endif //CACHE_SIMULATOR_RESULT_H