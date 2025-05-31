#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "types.h"
#include <fstream>
#include <vector>

struct BMPFileHeader
{
    u_int16_t bfType = 0x4D42; // 'BM'
    u_int32_t bfSize = 0;
    u_int16_t bfReserved1 = 0;
    u_int16_t bfReserved2 = 0;
    u_int32_t bfOffBits = 54; // Header size
};

struct BMPInfoHeader
{
    u_int32_t biSize = 40;             // size of this header, 40 bytes
    u_int16_t biWidth = 0;             // width in pixels
    u_int16_t biHeight = 0;            // height in pixels
    u_int16_t biColourPanes = 1;       // number of colour planes - has to be 1
    u_int16_t biBitsPerPixel = 24;     // number of bits per pixel
    u_int32_t biCompressionMethod = 0; // compression method - none
    u_int32_t biImageSize = 0;         // image size
    int32_t biXPixelsPerMeter = 2835;  // pixels per meter width
    int32_t biYPixelPerMeter = 2835;   // pixels per meter height
    u_int32_t biColourPallet = 0;      // colour pallet
    u_int32_t biImportantColours = 0;  // important colours
};

class BmpWriter
{
public:
    BmpWriter();
    ~BmpWriter();

    void write(u_int16_t data, size_t size);
    void writeHeader(int w, int h);
    void writePixelData(const std::vector<std::vector<Pixel>> &pixels);

private:
    std::ofstream outputStream;
    int width = 0;
    int height = 0;

    void writeFileHeader(BMPFileHeader header);
    void writeInfoHeader(BMPInfoHeader header);
};

#endif