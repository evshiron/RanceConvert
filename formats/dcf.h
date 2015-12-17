//
// Created by evshiron on 12/17/15.
//

#ifndef RANCECONVERT_DCF_H
#define RANCECONVERT_DCF_H

#include <string>

#include "qnt.h"

class DCF {

private:

    u_char* mBytes;
    size_t mSize;

public:

    string Path;
    string Filename;

    QNT* Qnt;

    DCF();
    void Load(string path);
    void Load(u_char* bytes, size_t size);
    ~DCF();

};


#endif //RANCECONVERT_DCF_H
