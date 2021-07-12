# 2D Template

This template has:
- 2D Projection
- Clears background
- 3 sprites with different effects

## How to Build

You can run `build.sh` and it will rebuild the whole project everytime if you have the sdk installed, or use `docker-prepare.sh` (to build the image) and `docker-build.sh` (to build the project) to use Docker instead.

If you are using VSCode, this project comes with a pre-configured task that will run the bash file when you press `CTRL+SHIFT+B`.

## Project Walkthrough

This project started from the N64SDK sample "N0" and uses NuSystem.

Source code and Makefile are in the `src/` folder, and the textures built during the build are in `resources/`.

Inside you have the `.asm` files inside the `asm/` folder, `data` files (code with the textures) inside the `data/` folder, and the `screens/` folder has the scene used by NuSystem.

The data folder contains 2 types of textures, `spr_pyoro_walk1` that is a single texture that was already transformed to an N64 texture (texture data is inside `spr_pyoro_walk1.c`), and `spr_peons` that is built when running the `build-resources.sh` file (that is called inside `build.sh`) from `resources/gfx/peons/peons.png`.

The `main.c` file has the start function called by NuSystem, and calls `stage00_init()` when starting, and `stage00_update()` and `stage00_draw()` every frame.

`graphic.c` has helper functions to initialize the RSP and RDP.

`stage00.c` is the file with the actual implementation of the game, with the function `stage00_update()` handling updates to the state of the game, and `stage00_draw()` handling the drawing. It has four functions (`draw_pyoro()`, `draw_pyoro_scaled()`, and `draw_pyoro_rotated()`) that will draw a sprite to the screen, with `draw_pyoro()` and `draw_peon()` being the simplests you can use to start using your custom textures with.

If you are using NuDebCon (the debug console that comes with NuSystem), you can uncomment the last section inside `stage00_draw()` to draw what you set on the screen (remember to uncomment both lines).
