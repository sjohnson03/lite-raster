#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include "types.h"
#include <string>

namespace LiteRaster {
class Texture {
public:
  int width, height, channels;
  unsigned char *data;

  Texture(const std::string &fileName);
  ~Texture();

  Colour sample(float u, float v) const;
};
} // namespace LiteRaster

#endif