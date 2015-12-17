//
// Created by evshiron on 12/16/15.
//

#ifndef RANCECONVERT_COMMON_H
#define RANCECONVERT_COMMON_H

#include <iostream>
#include <string>

#define FATAL(x) { std::cerr << x << std::endl; exit(1); }

using namespace std;

namespace Common {

    size_t ReadFile(string path, u_char** outBytes) {

        FILE* file = fopen(path.c_str(), "rb");
        if(!file) FATAL("ERROR_FOPEN_FAILED");

        fseek(file, 0L, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        u_char* bytes = new u_char[size];

        size_t s = fread(bytes, 1, size, file);
        if(size != s) FATAL("ERROR_FSIZE_MISMATCH");

        fclose(file);

        *outBytes = bytes;

        return s;

    }

}

#endif //RANCECONVERT_COMMON_H
