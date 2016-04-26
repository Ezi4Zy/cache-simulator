//
// Created by fear on 16-4-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "result_file.h"

using std::string;

result_file::result_file(const string &filename) {
	file = fopen(filename.c_str(),"w");
	if(file == NULL){
		fputs (" Open file error",stderr);
		exit (1);
	}
}

result_file::~result_file() {
	fclose(file);
}

void result_file::write_line(const std::string& line) {
	if (line.size() > 0){
		if(fwrite(line.c_str(), line.size(), 1, file) != 1){
			fputs (" Write file error",stderr);
		}
	}
}
