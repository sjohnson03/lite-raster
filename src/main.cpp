#include "main.h"

int main()
{
    int width = 640;
    int height = 360;
    // screen space is from [-1, 1] for both width and height
    Scene scene = Scene();

    Object cube("example/cube.obj");
    cube.setScale(float3(0.5f, 0.5f, 0.5f));

    scene.add(cube);

    for (int frames = 0; frames < 61; frames++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto pixels = scene.render(width, height);

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