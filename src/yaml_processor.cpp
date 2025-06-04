#include "yaml_processor.h"

void loadSceneFromYAML(const std::string &filename, Scene &scene)
{
    YAML::Node root = YAML::LoadFile(filename);

    for (const auto &node : root["scene"])
    {
        std::string name = node["object"].as<std::string>();
        std::string file = node["file"].as<std::string>();
        auto pos = node["position"];
        auto rot = node["rotation"];
        auto scl = node["scale"];

        Object *obj = new Object(file);

        obj->setTransform(float3(pos[0].as<float>(), pos[1].as<float>(), pos[2].as<float>()));
        obj->setRotation(float3(rot[0].as<float>(), rot[1].as<float>(), rot[2].as<float>()));
        obj->setScale(float3(scl[0].as<float>(), scl[1].as<float>(), scl[2].as<float>()));

        obj->name = name;

        scene.add(*obj);
    }
}