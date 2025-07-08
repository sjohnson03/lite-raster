#ifndef YAML_PROCESSOR_H
#define YAML_PROCESSOR_H

#include "light.h"
#include "object.h"
#include "scene.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

void loadSceneFromYAML(const std::string &filename, Scene &scene);

#endif