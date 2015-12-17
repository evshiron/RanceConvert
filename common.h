//
// Created by evshiron on 12/16/15.
//

#ifndef RANCECONVERT_COMMON_H
#define RANCECONVERT_COMMON_H

#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>

#define FATAL(x) { std::cerr << x << std::endl; exit(1); }

using namespace std;

namespace Common {

    size_t ReadFile(string path, u_char** outBytes);

    // Functions for outputing RGBA data to a file with specific format.
    void OutputP3PPM(string path, int width, int height, u_char* data);
    void OutputP7PPM(string path, int width, int height, u_char* data);
    void OutputPNG(string path, int width, int height, u_char* data);

}

#endif //RANCECONVERT_COMMON_H
