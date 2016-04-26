//
// Created by fear on 16-4-26.
//

#ifndef CACHE_SIMULATOR_RESULT_FILE_H
#define CACHE_SIMULATOR_RESULT_FILE_H

#include <string>
class result_file{
public:
	explicit result_file(const std::string& filename);
	~result_file();
	void write_line(const std::string& line);
private:
	FILE* file;
};

#endif //CACHE_SIMULATOR_RESULT_FILE_H
