#include "main.h"

int main()
{
    int width = 200;
    int height = 200;
    // screen space is from [-1, 1] for both width and height

    // Initialize pixel grid with black pixels
    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width));
    Scene scene = Scene();

    Object cube("example/cube.obj");
    scene.add(cube);

    std::vector<Triangle> triangles;

    // Render loop

    for (unsigned long i = 0; i < scene.objects.size(); i++) // project all triangles
    {
        Object *object = scene.objects[i].first;
        if (object)
        {
            for (unsigned long j = 0; j < object->triangles.size(); j++)
            {
                Triangle triangle = object->triangles[j]->projectTo2D(width, height);

                triangles.push_back(triangle);

                std::cout
                    << "Triangle vertices: "
                    << "(" << triangle.A.x << ", " << triangle.A.y << "), "
                    << "(" << triangle.B.x << ", " << triangle.B.y << "), "
                    << "(" << triangle.C.x << ", " << triangle.C.y << ")" << std::endl;
            }
        }
    }

    // TODO - Move this to scene class
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float2 point = float2(x, y);
            // set each pixel individually

            for (unsigned long i = 0; i < triangles.size(); i++)
            {
                Triangle triangle = triangles[i];
                if (triangle.isPointInsideTriangle(point))
                {
                    pixels[y][x] = Pixel(triangle.getColour());
                }
            }
        }
    }

    BmpWriter bmpWriter;
    bmpWriter.writeHeader(width, height);
    bmpWriter.writePixelData(pixels);

    return 0;
}