//
// Created by evshiron on 12/17/15.
//

#include "common.h"

#include <IL/il.h>

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

    // Deprecated.
    void OutputP3PPM(string path, int width, int height, u_char* data) {

        ofstream ofs(path);

        ofs << "P3" << endl;
        ofs << width << " " << height << " " << 255 << endl;

        for(int i = height - 1; i >= 0; i--) {

            for(int j = 0; j < width; j++) {

                int k = i * width + j;

                ofs << (int) data[k*4+0] << " " << (int) data[k*4+1] << " " << (int) data[k*4+2] << endl;

            }

        }

        ofs.flush();
        ofs.close();

    }

    // Deprecated.
    void OutputP7PPM(string path, int width, int height, u_char* data) {

        ofstream ofs(path);

        ofs << "P7" << endl;
        ofs << "WIDTH " << width << endl;
        ofs << "HEIGHT " << height << endl;
        ofs << "DEPTH " << 4 << endl;
        ofs << "MAXVAL " << 255 << endl;
        ofs << "TUPLTYPE RGB_ALPHA" << endl;
        ofs << "ENDHDR" << endl;

        for(int i = height - 1; i >= 0; i--) {

            for(int j = 0; j < width; j++) {

                int k = i * width + j;

                ofs << data[k*4+0] << data[k*4+1] << data[k*4+2] << data[k*4+3];

            }

        }

        ofs.flush();
        ofs.close();

    }

    void OutputPNG(string path, int width, int height, u_char* data) {

        ILuint image = 0;
        ilGenImages(1, &image);
        ilBindImage(image);

        ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, data);

        ilSave(IL_PNG, path.c_str());

        ilBindImage(0);
        ilDeleteImage(image);

    }

}