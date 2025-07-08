#include "yaml_processor.h"
#include "types.h"
#include <cstdint>
#include <sys/types.h>

void loadSceneFromYAML(const std::string &filename, Scene &scene) {
  YAML::Node root = YAML::LoadFile(filename);

  // Load objects
  if (root["object"]) {
    for (const auto &node : root["object"]) {
      std::string name = node["name"].as<std::string>();
      std::string file = node["file"].as<std::string>();
      auto pos = node["position"];
      auto rot = node["rotation"];
      auto scl = node["scale"];

      Object *obj = new Object(file);
      obj->setTransform(
          float3(pos[0].as<float>(), pos[1].as<float>(), pos[2].as<float>()));
      obj->setRotation(
          float3(rot[0].as<float>(), rot[1].as<float>(), rot[2].as<float>()));
      obj->setScale(
          float3(scl[0].as<float>(), scl[1].as<float>(), scl[2].as<float>()));
      obj->name = name;

      scene.add(*obj);
    }
  }

  // Load lights
  if (root["light"]) {
    for (const auto &node : root["light"]) {
      std::string name = node["name"].as<std::string>();
      auto pos = node["position"];
      auto col = node["color"];
      float intensity = node["intensity"].as<float>();
      float ambient = node["ambient"].as<float>();

      Light *light = new Light;
      light->position =
          (float3(pos[0].as<float>(), pos[1].as<float>(), pos[2].as<float>()));
      light->colour = Colour(col[0].as<uint8_t>(), col[1].as<uint8_t>(),
                             col[2].as<uint8_t>());
      light->intensity = (intensity);
      light->ambient = ambient;
      light->name = name;

      scene.add(*light);
    }
  }
}
