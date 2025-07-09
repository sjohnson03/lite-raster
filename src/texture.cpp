#include "texture.h"
#include "types.h"
#include <algorithm>

namespace LiteRaster {
Texture::Texture(const std::string &fileName) {
  data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
}

Texture::~Texture() {
  if (data) {
    stbi_image_free(data);
  }
}

Colour Texture::sample(float u, float v) const {
  int x = std::clamp(int(u * width), 0, width - 1);
  int y = std::clamp(int(v * height), 0, height - 1);
  int idx = (y * width + x) * channels;

  return Colour(data[idx], data[idx + 1], data[idx + 2]);
}
} // namespace LiteRaster
