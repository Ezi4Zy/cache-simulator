//
// Created by fear on 2016/4/24.
//

#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include "trace_file.h"

trace_file::trace_file(const string& filename) {
    file = fopen(filename.c_str(),"r");
    if(file == NULL){
        fputs (" Open file error",stderr);
        exit (1);
    }
    line_index = 0;
}

trace_file::~trace_file() {
    fclose(file);
    lines.clear();
}

void trace_file::read_file() {
    //obtain the file size
    fseek(file, 0 , SEEK_END);
    long file_size = ftell (file);
    rewind (file);

    // allocate memory to contain the whole file:
    char* buffer = (char*) malloc (sizeof(char)*file_size);
    if (buffer == NULL) {
        fputs ("Memory error",stderr);
        exit (2);
    }

    // copy the file into the buffer:
    int read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        fputs("Read file error", stderr);
        exit(3);
    }
    char* begin = buffer;
    char* end = buffer;
    size_t index = 0;
    lines.resize(file_size/15);
    while(end <= buffer + read_size - 1){
        if (*end != '\n'){
            end++;
            continue;
        } else{
            lines[index++].assign(begin, end - begin);
            begin = ++end;
        }
    }
    line_index = 0;
    lines.resize(index);
    free(buffer);
}

string trace_file::read_line() {
    if(line_index < lines.size())
        return lines[line_index++];
    else
        return "";
}