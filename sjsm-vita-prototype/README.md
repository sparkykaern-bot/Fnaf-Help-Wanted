# Spooky Corridor Vita — technical prototype

This is the first native PS Vita build target for investigating a possible *Spooky's Jump Scare Mansion* port. It is **not the original game** and does not contain its copyrighted code, art, audio, or levels.

## Current milestone

- VitaSDK + SDL2 application scaffold.
- 960×544 first-person corridor-style rendering test.
- Analog-stick movement and turning when SDL recognizes the controller.
- Keyboard controls are available when running on a desktop SDL build for quick logic testing.
- GitHub Actions workflow produces a VPK artifact if the toolchain build succeeds.

## Controls

- Left stick up/down: move forward/back.
- Left stick left/right: turn.
- START: quit.
- Desktop testing: WASD or arrow keys; ESC quits.

## Build

Requires VitaSDK and its SDL2 package:

```sh
vdpm install sdl2
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The VPK should be named `SpookyCorridorVita.vpk` in the build directory.

## What this does not do yet

This is only a renderer/input proof of concept. It does not load the original game, reproduce its story, include the 1,000 rooms, or implement enemy AI, saves, inventory, sound, and original assets. The original game's source/assets or a legally usable project and assets are needed to pursue a faithful port. The next milestone is verifying the VPK on actual Vita hardware, then replacing the placeholder corridor with a proper low-poly renderer and resource pipeline.
