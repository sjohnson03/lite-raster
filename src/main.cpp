#include "main.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <path-to-yaml-file>\n";
    return 1;
  }

  int width = 960;
  int height = 540;
  // screen space is from [-1, 1] for both width and height
  Scene scene;

  const char *yamlFilePath = argv[1];

  loadSceneFromYAML(yamlFilePath, scene);

  // Ray lib
  InitWindow(width, height, "LiteRaster");
  SetTargetFPS(60);

  Color *pixelBuffer =
      new Color[width * height]; // stores data about all pixels in the scene
  Texture2D texture = LoadRenderTexture(width, height).texture;

  while (!WindowShouldClose()) {
    auto start = std::chrono::high_resolution_clock::now();
    float deltaTime =
        GetFrameTime(); // for more consistent movement independent of FPS

    // rotate suzanne
    auto it = scene.addedObjects.find("suzanne");
    if (it != scene.addedObjects.end()) {
      it->second->addRotation(float3(0.0f, 50 * deltaTime, 0.0f));
    }

    scene.render(width, height, pixelBuffer);

    UpdateTexture(texture, pixelBuffer); // upload CPU-rendered pixels to GPU

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(texture, 0, 0,
                WHITE); // tells GPU to draw the texture to the screen
    EndDrawing();

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\rFrame rendered in " << duration.count() << " ms"
              << std::flush; // measure time to render each frame
  }

  delete[] pixelBuffer;
  CloseWindow();

  return 0;
}