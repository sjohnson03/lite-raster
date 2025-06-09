# lite-raster

**lite-raster** is a lightweight rasterization project designed for efficient 3D rendering tasks. It provides a simple way to define scenes and render raster images, making it ideal for graphics projects, educational purposes, or experimentation. This project was created as an educational challenge to better understand the fundamentals of computer graphics.

## Features

- Fast and minimal rasterization core
- Support for basic geometric primitives
- Simple transformation and colour handling
- Standalone usage - no integration required

## Building

### Prerequisites

- C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.10 or higher

### Build Instructions

```bash
git clone https://github.com/sjohnson03/lite-raster.git
cd lite-raster
mkdir build
cd build
cmake ..
cmake --build .
```

This will generate the executable binaries in the `bin/` directory.

## Usage

To use **lite-raster**, define your scene in the `scene.yaml` file located in the project root. The application reads this file to render the specified scene.

The binary must be called from the project root to ensure that `scene.yaml` is loaded correctly.

Assuming a structure like:

```bash
lite-raster/
├── bin/
│   └── lite-raster # built binary
├── build/
│   └── ...
├── include/
│   └── ...
├── objects/
├── src/
├── scene.yaml     # YAML scene file
└── CMakeLists.txt
```
The program can be run from the project root with:
```bash
./bin/lite-raster
```

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.