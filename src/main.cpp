#include "main.h"

int main()
{
    int width = 960;
    int height = 540;
    // screen space is from [-1, 1] for both width and height
    Scene scene;

    loadSceneFromYAML("scene.yaml", scene);

    // Ray lib
    InitWindow(width, height, "LiteRaster");
    SetTargetFPS(60);

    Color *pixelBuffer = new Color[width * height]; // stores data about all pixels in the scene
    Texture2D texture = LoadRenderTexture(width, height).texture;

    auto *suzanneObj = scene.addedObjects.at("suzanne");
    suzanneObj->setColour(235, 171, 52); // add a custom colour

    while (!WindowShouldClose())
    {
        auto start = std::chrono::high_resolution_clock::now();
        float deltaTime = GetFrameTime(); // for more consistent movement independent of FPS

        // update scene
        scene.addedObjects.at("suzanne")->addRotation(float3(0.0f, 50.0f * deltaTime, 0.0f));

        scene.render(width, height, pixelBuffer);

        UpdateTexture(texture, pixelBuffer); // upload CPU-rendered pixels to GPU

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE); // tells GPU to draw the texture to the screen
        EndDrawing();

        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "\rFrame rendered in " << duration.count() << " ms" << std::flush; // measure time to render each frame
    }

    delete[] pixelBuffer;
    CloseWindow();

    return 0;
}