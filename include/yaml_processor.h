#ifndef YAML_PROCESSOR_H
#define YAML_PROCESSOR_H

#include <yaml-cpp/yaml.h>
#include <iostream>
#include "object.h"
#include "scene.h"

void loadSceneFromYAML(const std::string &filename, Scene &scene);

#endif