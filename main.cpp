
#include <iostream>

#include <IL/il.h>

#include "common.h"
#include "formats/qnt.h"

using namespace std;

int main(int argc, char** argv) {

    //cout << "Hello, World!" << endl;

    ilInit();

    cout << "argc: " << argc << endl;

    for(int i = 0; i < argc; i++) {

        cout << "argv[" << i << "]: " << argv[i] << endl;

    }

    for(int i = 1; i < argc; i++) {

        QNT qnt;
        qnt.Load(string(argv[i]));

        string path = qnt.Path + ".png";

        cout << "> " << path << endl;

        //Common::OutputP3PPM(qnt.Path + ".p3.ppm", qnt.Info->width, qnt.Info->height, qnt.GetData());
        //Common::OutputP7PPM(qnt.Path + ".p7.ppm", qnt.Info->width, qnt.Info->height, qnt.GetData());
        Common::OutputPNG(path, qnt.Info->width, qnt.Info->height, qnt.GetData());

        cout << "Saved." << endl;

    }

    return 0;

}