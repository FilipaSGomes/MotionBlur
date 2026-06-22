# VI-RT-V4-PathTracing

C++ ray tracing project based on the VI-RT framework, extended with distributed rendering, path tracing, depth of field, and motion blur support.

The current executable renders a 640x640 `BallBoxRevealScene` using the `PathTracing` shader with 128 samples per pixel and writes the result to `MyImage.ppm`.

## Features

- Perspective camera with optional defocus blur
- Whitted, distributed, ambient, dummy, and path tracing shaders
- Sphere and triangle primitives
- Area, point, and ambient lights
- Texture-backed diffuse materials using PPM images
- Per-ray shutter time for motion blur
- Axis-aligned bounding boxes for static and moving primitives
- PPM image output

## Repository Layout

```text
VI-RT-V4-PathTracing/       Source code and input textures
VI-RT-V4-PathTracing.xcodeproj/
                             Xcode project files
Makefile                     Command-line build
relatorio_sem_animacao.typ   Project report
results/                     Generated renders, ignored by Git
build/                       Build output, ignored by Git
```

## Build

From the repository root:

```sh
make
```

This creates:

```text
build/apps/VI-RT-V4-PathTracing
```

The Makefile also copies the input `.ppm` textures into `build/apps/`, so running the program from that directory is the most reliable option when a scene uses textures.

## Run

```sh
cd build/apps
./VI-RT-V4-PathTracing
```

The render is saved as:

```text
build/apps/MyImage.ppm
```

Rendering progress is printed by row, and the total rendering time is printed when the run finishes.

## Change The Render

Most render settings are currently selected directly in `VI-RT-V4-PathTracing/main.cpp`:

- Image resolution: `W` and `H`
- Scene: calls such as `BallBoxRevealScene(scene)`, `CornellBox(scene)`, or `DeformCornellBox(scene)`
- Camera position and focus settings: `Eye`, `At`, `deFocusRad`, and `FocusDist`
- Shader: `PathTracing`, `DistributedShader`, `WhittedShader`, etc.
- Samples per pixel: `spp`
- Pixel jittering: `jitter`

After changing those values, rebuild with `make`.

## Clean

```sh
make clean
```

## Notes

Generated renders, animation files, build artifacts, `.DS_Store`, and local Xcode user state are ignored by Git. Source files, the report, project metadata, and input texture assets are tracked.
