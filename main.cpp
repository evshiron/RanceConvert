
#include <iostream>

#include "formats/qnt.h"

using namespace std;

int main(int argc, char** argv) {

    //cout << "Hello, World!" << endl;

    cout << "argc: " << argc << endl;

    for(int i = 0; i < argc; i++) {

        cout << "argv[" << i << "]: " << argv[i] << endl;

    }

    for(int i = 1; i < argc; i++) {

        QNT qnt;
        qnt.Load(string(argv[i]));

        qnt.OutputP7PPM();

    }

    return 0;

}