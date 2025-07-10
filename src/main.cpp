#include "main.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <path-to-yaml-file>\n";
    return 1;
  }

  int renderWidth = 960;
  int renderHeight = 540;
  // screen space is from [-1, 1] for both width and height
  Scene scene;

  const char *yamlFilePath = argv[1];

  loadSceneFromYAML(yamlFilePath, scene);

  // Ray lib
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(renderWidth, renderHeight, "LiteRaster");
  SetTargetFPS(60);

  Color *pixelBuffer =
      new Color[renderWidth * renderHeight]; // stores data about all pixels in the scene
  RenderTexture2D renderTarget = LoadRenderTexture(renderWidth, renderHeight);

  while (!WindowShouldClose()) {
    auto start = std::chrono::high_resolution_clock::now();
    float deltaTime =
        GetFrameTime(); // for more consistent movement independent of FPS

    // rotate suzanne
    auto it = scene.addedObjects.find("suzanne");
    if (it != scene.addedObjects.end()) {
      it->second->addRotation(float3(0.0f, 50 * deltaTime, 0.0f));
    }

    scene.render(renderWidth, renderHeight, pixelBuffer);

    UpdateTexture(renderTarget.texture, pixelBuffer); // upload CPU-rendered pixels to GPU

    BeginDrawing();
    ClearBackground(BLACK);
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    Rectangle sourceRect = {};
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.width = (float)renderWidth;
    sourceRect.height = (float)renderHeight;

    Rectangle destRect = {};
    destRect.x = 0;
    destRect.y = 0;
    destRect.width = (float)windowWidth;
    destRect.height = (float)windowHeight;

    Vector2 origin = { 0.0f, 0.0f };

    // this makes the ray lib window scalable whilst maintaining the same render size
    DrawTexturePro(
      renderTarget.texture,
      sourceRect,
      destRect,
      origin,
      0.0f,
      WHITE
    );
    EndDrawing();

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\rFrame rendered in " << duration.count() << " ms"
              << std::flush; // measure time to render each frame
  }

  delete[] pixelBuffer;
  UnloadRenderTexture(renderTarget);
  CloseWindow();

  return 0;
}