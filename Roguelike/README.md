# Roguelike Template

This template has:
- Grid-based movement
- Console-style graphics (uses a tileset you can replace with actual 6x6 images)
- Helper methods ported from [libtcod](https://github.com/libtcod/libtcod)

## How to Build

You can run `build.sh` and it will rebuild the whole project everytime if you have the sdk installed, or use `docker-prepare.sh` (to build the image) and `docker-build.sh` (to build the project) to use Docker instead.

If you are using VSCode, this project comes with a pre-configured task that will run the bash file when you press `CTRL+SHIFT+B`.

## Project Walkthrough

This project started from the template "2D" and uses NuSystem.

Source code and Makefile are in the `src/` folder, and the textures built during the build are in `resources/`.

TODO: ADD MORE INFO HERE

If you are using NuDebCon (the debug console that comes with NuSystem), you can uncomment the last section inside `stage00_draw()` to draw what you set on the screen (remember to uncomment both lines).
