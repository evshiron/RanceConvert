//
// Created by evshiron on 12/16/15.
//

#ifndef RANCECONVERT_QNT_H
#define RANCECONVERT_QNT_H

#include <sys/types.h>
#include <string>

#define QNT_MARGIN 4096

using namespace std;

struct QNTInfo {

    int32_t reserved1;
    int32_t reserved2;
    int32_t width;
    int32_t height;
    int32_t bitCount;
    int32_t reserved3;
    int32_t imageSize;
    int32_t alphaSize;

};

class QNT {

private:

    size_t mSize;
    u_char* mBytes;
    u_char* mImage;

public:

    string Path;
    string Filename;
    QNTInfo* Info;

    QNT();
    void Load(string path);
    void OutputP3PPM();
    void OutputP7PPM();
    ~QNT();

};


#endif //RANCECONVERT_QNT_H
