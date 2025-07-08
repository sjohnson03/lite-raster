#ifndef LIGHT_H
#define LIGHT_H

#include "types.h"
#include <string>

struct Light {
  float3 position;
  Colour colour;
  float intensity;
  float ambient;
  std::string name;

  Light()
      : position(float3(0.f, 0.f, 0.f)), colour(Colour(0, 0, 0)),
        intensity(0.f) {}
};

#endif