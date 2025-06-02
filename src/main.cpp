#include "main.h"

int main()
{
    int width = 200;
    int height = 200;
    // screen space is from [-1, 1] for both width and height
    Scene scene = Scene();

    Object cube("example/cube.obj");
    cube.setScale(float3(0.5f, 0.5f, 0.5f));

    scene.add(cube);

    std::vector<Triangle> triangles;

    for (int frames = 0; frames < 61; frames++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width)); // clear the pixel buffer
        triangles.clear();                                                         // clear all triangles

        for (unsigned long i = 0; i < scene.objects.size(); i++) // project all triangles
        {
            Object *object = scene.objects[i].first;
            if (object)
            {
                for (unsigned long j = 0; j < object->triangles.size(); j++)
                {
                    Triangle triangle = object->triangles[j]->projectTo2D(width, height);

                    triangles.push_back(triangle);
                }
            }
        }

        // actual render loop - TODO - move this to scene class
        for (const Triangle &triangle : triangles)
        {
            // only draw where triangles actually are
            auto [minX, maxX, minY, maxY] = triangle.getBoundingBox(width, height);

            for (int y = minY; y <= maxY; ++y)
            {
                for (int x = minX; x <= maxX; ++x)
                {
                    float2 point = float2(x, y);
                    // set each pixel individually

                    if (triangle.isPointInsideTriangle(point))
                    {
                        pixels[y][x] = Pixel(triangle.getColour());
                    }
                }
            }
        }

        BmpWriter bmpWriter("out" + std::to_string(frames) + ".bmp");
        bmpWriter.writeHeader(width, height);
        bmpWriter.writePixelData(pixels);

        cube.setRotation(float3(0.0f, 2 * frames, 0.0f));

        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Frame " << frames << " rendered in " << duration.count() << " ms" << std::endl; // measure time to render each frame
    }

    return 0;
}