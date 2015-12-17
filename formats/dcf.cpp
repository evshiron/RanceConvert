//
// Created by evshiron on 12/17/15.
//

#include "dcf.h"

#include <libgen.h>

#include "../common.h"

DCF::DCF() {

    Qnt = 0;

}

void DCF::Load(string path) {

    Path = path;
    Filename = string(basename((char *) Path.c_str()));

    cout << "< " << Path << endl;

    u_char* bytes = 0;
    size_t size = 0;

    size = Common::ReadFile(Path, &bytes);

    Load(bytes, size);

}

void DCF::Load(u_char* bytes, size_t size) {

    if(Path.empty()) {

        Path = "MEMORY";
        Filename = "MEMORY";
        cout << "= " << Path << endl;

    }

    mBytes = bytes;
    mSize = size;

    int qntStart = 0;
    for(int i = 4; i < size - 2; i++) {

        if(memcmp(&mBytes[i], "QNT", 3) == 0) {

            qntStart = i;
            break;

        }

    }

    if(qntStart == 0) FATAL("ERROR_QNT_NOT_FOUND");

    Qnt = new QNT();
    // QNT borrows mBytes.
    Qnt->Load(&mBytes[qntStart], mSize - qntStart);

}

DCF::~DCF() {

    delete[] mBytes;

    delete Qnt;

}
