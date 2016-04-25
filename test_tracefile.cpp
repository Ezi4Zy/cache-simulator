//
// Created by fear on 2016/4/24.
//
#include "trace_file.h"
#include <iostream>
using std::cout;
using std::endl;
int main(){
    trace_file tf("test.trace");
    tf.read_file();
    while(1){
        string line = tf.getline();
        if(line == "")
            break;
        cout << line << endl;
    }
}

