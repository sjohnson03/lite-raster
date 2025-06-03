#include "main.h"

int main()
{
    int width = 640;
    int height = 360;
    // screen space is from [-1, 1] for both width and height
    Scene scene = Scene();

    Object cube("objects/cube.obj");
    cube.setScale(float3(0.5f, 0.5f, 0.5f));

    scene.add(cube);

    // Ray lib
    InitWindow(width, height, "LiteRaster");
    SetTargetFPS(60);

    Color *buffer = new Color[width * height];
    Texture2D texture = LoadRenderTexture(width, height).texture;

    while (!WindowShouldClose())
    {
        auto start = std::chrono::high_resolution_clock::now();

        // update scene
        cube.addRotation(float3(1.0f, 1.0f, 1.0f));

        scene.render(width, height, buffer);

        UpdateTexture(texture, buffer); // upload CPU-rendered pixels to GPU

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE); // tells GPU to draw the texture to the screen
        EndDrawing();

        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Frame rendered in " << duration.count() << " ms" << std::endl; // measure time to render each frame
    }

    delete[] buffer;
    CloseWindow();

    return 0;
}