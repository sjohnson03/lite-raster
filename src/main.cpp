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

    Color *buffer = new Color[width * height];
    Texture2D texture = LoadRenderTexture(width, height).texture;

    while (!WindowShouldClose())
    {
        auto start = std::chrono::high_resolution_clock::now();

        // update scene
        scene.addedObjects.at("suzanne")->addRotation(float3(0.0f, 1.0f, 0.0f));

        scene.render(width, height, buffer);

        UpdateTexture(texture, buffer); // upload CPU-rendered pixels to GPU

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE); // tells GPU to draw the texture to the screen
        EndDrawing();

        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "\rFrame rendered in " << duration.count() << " ms" << std::flush; // measure time to render each frame
    }

    delete[] buffer;
    CloseWindow();

    return 0;
}