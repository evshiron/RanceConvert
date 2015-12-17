//
// Created by evshiron on 12/16/15.
//

#include "qnt.h"

#include <iostream>
#include <fstream>

#include <libgen.h>
#include <zlib.h>

#include "../common.h"

#define BLEND(a, b, alpha) (((a - b) * (alpha + 1) >> 8) + b)

using namespace std;

QNT::QNT() {

    mBytes = 0;
    mImage = 0;

    Info = 0;

}

void QNT::Load(string path) {

    Path = path;
    Filename = string(basename((char*) Path.c_str()));

    cout << "< " << path << endl;

    mSize = Common::ReadFile(path, &mBytes);

    int32_t version = *reinterpret_cast<int32_t*>(&mBytes[4]);

    cout << "version: " << version << endl;

    int32_t headerSize = 0;

    if(version == 0) {

        headerSize = 48;
        Info = reinterpret_cast<QNTInfo*>(&mBytes[8]);

    }
    else {

        headerSize = *reinterpret_cast<int32_t*>(&mBytes[8]);
        Info = reinterpret_cast<QNTInfo*>(&mBytes[12]);

    }

    cout << "width: " << Info->width << endl;
    cout << "height: " << Info->height << endl;
    cout << "bitCount: " << Info->bitCount << endl;
    cout << "imageSize: " << Info->imageSize << endl;
    cout << "alphaSize: " << Info->alphaSize << endl;

    if(Info->bitCount != 24 || Info->imageSize == 0) FATAL("ERROR_UNKNOWN_FORMAT");

    // RGB.
    {

        cout << "RGB handle starts." << endl;

        // Packed row bytes with 4-byte alignment.
        int rowBytes = (Info->width * 3 + 3) & ~3;
        unsigned long destSize = rowBytes * Info->height + QNT_MARGIN;

        cout << "rawBytes: " << Info->width * 3 << endl;
        cout << "rowBytes: " << rowBytes << endl;

        cout << "bufferSize: " << destSize << endl;

        u_char* image = new u_char[destSize];

        u_char* data = new u_char[destSize];
        if(uncompress(data, &destSize, &mBytes[headerSize], Info->imageSize) != Z_OK) FATAL("ERROR_UNCOMPRESS_IMAGE");

        cout << "uncompressedSize: " << destSize << endl;

        // BGR, BGR, ... のオーダーに並び替え

        // 上下逆転させる
        u_char *ti = &image[rowBytes * (Info->height - 1)];
        u_char *tj = data;
        for(int color = 0; color < 3; color++) {
            u_char *dest = &ti[color];
            for(long i = 0; i < Info->height - 1; i += 2) {
                u_char *dest1 = dest;
                u_char *dest2 = dest - rowBytes;
                for(long j = 0; j < Info->width - 1; j += 2) {
                    dest1[0] = *tj++;
                    dest2[0] = *tj++;
                    dest1[3] = *tj++;
                    dest2[3] = *tj++;
                    dest1 = &dest1[6];
                    dest2 = &dest2[6];
                }
                // 奇数だと末尾にダミーが入っている
                if(Info->width & 1) {
                    dest1[0] = tj[0];
                    dest2[0] = tj[1];
                    tj = &tj[4];
                }
                dest -= rowBytes * 2;
            }
            // 奇数だと末尾にダミー行が入っている
            if(Info->height & 1) {
                for(long j = 0; j < Info->width - 1; j += 2) {
                    dest[0] = tj[0];
                    dest[3] = tj[2];
                    dest = &dest[6];
                    tj = &tj[4];
                }
                if(Info->width & 1) {
                    dest[0] = tj[0];
                    tj = &tj[4];
                }
            }
        }
        // 上と左のピクセルとの色差分を取る
        u_char *dest = ti;
        for(long j = 0; j < Info->width - 1; j++) {
            // 1行目は左との差分のみ
            dest[3] = dest[0] - dest[3];
            dest[4] = dest[1] - dest[4];
            dest[5] = dest[2] - dest[5];
            dest = &dest[3];
        }

        dest = ti;
        for(long i = 0; i < Info->height - 1; i++) {
            // 2行目以降
            u_char *dest1 = dest;
            dest -= rowBytes;
            u_char *dest2 = dest;
            dest2[0] = dest1[0] - dest2[0];
            dest2[1] = dest1[1] - dest2[1];
            dest2[2] = dest1[2] - dest2[2];

            for(long j = 0; j < Info->width - 1; j++) {
                dest2[3] = (dest1[3] + dest2[0]) / 2 - dest2[3];
                dest2[4] = (dest1[4] + dest2[1]) / 2 - dest2[4];
                dest2[5] = (dest1[5] + dest2[2]) / 2 - dest2[5];
                dest1 = &dest1[3];
                dest2 = &dest2[3];
            }
        }

        delete[] data;

        cout << "RGB handle ends." << endl;

        if(Info->alphaSize)
        {

            // rowBytes (used), destSize (redefined), image (used as ti), data (redefined), dest (redefined), ti (copied as dest), tj (unused).
            // Should not be a problem here?

            cout << "Alpha handle starts." << endl;

            unsigned long destSize = (Info->width + 1) * Info->height + QNT_MARGIN;

            cout << "bufferSize: " << destSize << endl;

            u_char* data = new u_char[destSize];
            if(uncompress(data, &destSize, &mBytes[headerSize + Info->imageSize], Info->alphaSize) != Z_OK) FATAL("ERROR_UNCOMPRESS_ALPHA");

            cout << "uncompressedSize: " << destSize << endl;

            // 上と左のピクセルとの色差分を取る

            // 2バイト整合
            long width = (Info->width + 1) & ~1;
            u_char *tk = data;
            for(long j = 0; j < width - 1; j++) {
                // 1行目は左との差分のみ
                tk[j + 1] = tk[j] - tk[j + 1];
            }

            for(long i = 0; i < Info->height - 1; i++) {
                // 2行目以降
                u_char *prev = tk;
                tk = &tk[width];
                tk[0] = prev[0] - tk[0];

                for(long j = 0; j < width - 1; j++) {
                    tk[j + 1] = (tk[j] + prev[j + 1]) / 2 - tk[j + 1];
                }
            }

            // αブレンド
            tk = data;
            u_char* dest = ti;
            for(long i = 0; i < Info->height; i++) {
                dest = ti;
                for(long j = 0; j < Info->width; j++) {

                    if (tk[j] == 0xff) { }
                    else if (tk[j] == 0x00) {
                        dest[0] = 233;
                        dest[1] = 233;
                        dest[2] = 233;
                    }
                    else {
                        dest[0] = BLEND(dest[0], 233, tk[j]);
                        dest[1] = BLEND(dest[1], 233, tk[j]);
                        dest[2] = BLEND(dest[2], 233, tk[j]);
                    }
                    dest = &dest[3];
                }
                ti -= rowBytes;
                // 2バイト整合
                tk = &tk[(Info->width + 1) & ~1];
            }

            delete[] data;

            // TODO: Make (233, 233, 233) the transparent color or merge alpha into RGBA.
            cout << "Alpha handle ends." << endl;

        }

        // Now the image is the BGR data with 4-byte alignment per row, upside down.
        // Let's convert it to RGBA data, and store it into the mImage.

        mImage = new u_char[Info->width * Info->height * 4];

        int t = 0;

        for(int i = Info->height - 1; i >= 0; i--) {

            for(int j = 0; j < Info->width; j++) {

                int k = i * rowBytes + j * 3;

                mImage[t*4+0] = image[k+2];
                mImage[t*4+1] = image[k+1];
                mImage[t*4+2] = image[k+0];
                mImage[t*4+3] = 255;

                t++;

            }

        }

        delete[] image;

    }

}

void QNT::OutputP3PPM() {

    cout << "OutputP3PPM starts." << endl;

    string path = Path + ".p3.ppm";

    cout << "> " << path << endl;

    ofstream ofs(path);

    ofs << "P3" << endl;
    ofs << Info->width << " " << Info->height << " " << 255 << endl;

    for(int i = 0; i < Info->width * Info->height; i++) {

        ofs << (int) mImage[i*4+0] << " " << (int) mImage[i*4+1] << " " << (int) mImage[i*4+2] << endl;

    }

    ofs.flush();
    ofs.close();

    cout << "OutputP3PPM ends." << endl;

}

void QNT::OutputP7PPM() {

    cout << "OutputP7PPM starts." << endl;

    string path = Path + ".p7.ppm";

    cout << "> " << path << endl;

    ofstream ofs(path);

    ofs << "P7" << endl;
    ofs << "WIDTH " << Info->width << endl;
    ofs << "HEIGHT " << Info->height << endl;
    ofs << "DEPTH " << 4 << endl;
    ofs << "MAXVAL " << 255 << endl;
    ofs << "TUPLTYPE RGB_ALPHA" << endl;
    ofs << "ENDHDR" << endl;

    for(int i = 0; i < Info->width * Info->height; i++) {

        ofs << mImage[i*4+0] << mImage[i*4+1] << mImage[i*4+2] << mImage[i*4+3];

    }

    ofs.flush();
    ofs.close();

    cout << "OutputP7PPM ends." << endl;

}

QNT::~QNT() {

    delete[] mBytes;
    delete[] mImage;
    // Info is a pointer to mBytes'.
    //delete Info;

}
