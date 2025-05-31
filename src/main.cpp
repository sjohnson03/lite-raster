#include "main.h"

int main()
{
    int width = 200;
    int height = 200;

    // Initialize pixel grid with black pixels
    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width));
    Triangle tri = Triangle(float2(100, 10), float2(10, 190), float2(190, 190));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float2 point = float2(x, y);
            // set each pixel individually

            // Example: horizontal red gradient, vertical green gradient, and blue static
            float r = static_cast<float>(x) / (width - 1);  // left to right
            float g = static_cast<float>(y) / (height - 1); // top to bottom
            float b = 0.25f;                                // constant blue

            if (tri.isPointInsideTriangle(point))
            {
                Colour c(r, g, b);
                pixels[y][x] = Pixel(c);
            }
        }
    }

    BmpWriter bmpWriter;
    bmpWriter.writeHeader(width, height);
    bmpWriter.writePixelData(pixels);

    return 0;
}