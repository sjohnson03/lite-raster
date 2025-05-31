#include "output.h"

BmpWriter::BmpWriter() : outputStream("out.bmp", std::ios::out | std::ios::binary)
{
    if (!outputStream)
    {
        throw std::runtime_error("Failed to open output .bmp");
    }
}

BmpWriter::~BmpWriter()
{
    if (outputStream.is_open())
    {
        outputStream.close();
    }
}

void BmpWriter::write(u_int16_t data, size_t size)
{
    outputStream.write((char *)&data, size);
}

void BmpWriter::writeHeader(int w, int h)
{
    width = w;
    height = h;

    BMPFileHeader header;
    BMPInfoHeader infoHeader;

    int rowSize = ((width * 3 + 3) / 4) * 4; // width * number of colours aligned to 4-byte boundary
    int imageSize = rowSize * height;

    header.bfSize = header.bfOffBits + imageSize;

    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biImageSize = imageSize;

    writeFileHeader(header);
    writeInfoHeader(infoHeader);
}

void BmpWriter::writePixelData(const std::vector<std::vector<Pixel>> &pixels)
{
    // Each row must be padded to a multiple of 4 bytes
    int rowSize = ((width * 3 + 3) / 4) * 4;
    int paddingSize = rowSize - (width * 3);
    std::vector<unsigned char> padding(paddingSize, 0);

    for (int y = height - 1; y >= 0; --y) // BMP stores rows bottom-up
    {
        for (int x = 0; x < width; ++x)
        {
            const Colour &c = pixels[y][x].colour;

            // Clamp and convert float color [0.0, 1.0] → byte [0, 255]
            float r_clamped = c.r < 0.0f ? 0.0f : (c.r > 1.0f ? 1.0f : c.r);
            float g_clamped = c.g < 0.0f ? 0.0f : (c.g > 1.0f ? 1.0f : c.g);
            float b_clamped = c.b < 0.0f ? 0.0f : (c.b > 1.0f ? 1.0f : c.b);
            unsigned char r = static_cast<unsigned char>(r_clamped * 255);
            unsigned char g = static_cast<unsigned char>(g_clamped * 255);
            unsigned char b = static_cast<unsigned char>(b_clamped * 255);

            // Write in BGR order
            outputStream.put(b);
            outputStream.put(g);
            outputStream.put(r);
        }

        // Write padding
        outputStream.write(reinterpret_cast<char *>(padding.data()), paddingSize);
    }
}

void BmpWriter::writeFileHeader(BMPFileHeader header)
{
    outputStream.write((char *)&header.bfType, 2); // confirm type is correct
    write(header.bfSize, 4);                       // size of file
    write(header.bfReserved1, 2);                  // reserved
    write(header.bfReserved2, 2);                  // reserved
    write(header.bfOffBits, 4);                    // header size
}

void BmpWriter::writeInfoHeader(BMPInfoHeader header)
{
    write(header.biSize, 4);
    write(header.biWidth, 4);
    write(header.biHeight, 4);
    write(header.biColourPanes, 2);
    write(header.biBitsPerPixel, 2);
    write(header.biCompressionMethod, 4);
    write(header.biImageSize, 4);
    write(header.biXPixelsPerMeter, 4);
    write(header.biYPixelPerMeter, 4);
    write(header.biColourPallet, 4);
    write(header.biImportantColours, 4);
}
