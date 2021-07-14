# Roguelike Template

This template has:
- Console-style graphics on a 48x31 screen (uses a tileset you can replace with actual 6x6 images)
- Grid-based movement

## How to Build

You can run `build.sh` and it will rebuild the whole project everytime if you have the sdk installed, or use `docker-prepare.sh` (to build the image) and `docker-build.sh` (to build the project) to use Docker instead.

If you are using VSCode, this project comes with a pre-configured task that will run the bash file when you press `CTRL+SHIFT+B`.

## Project Walkthrough

This project started from the template "2D" and uses NuSystem.

Source code and Makefile are in the `src/` folder, and the textures built during the build are in `resources/`.

### Updating Font

The font code is inside `/fonts`, and you can replace the font by replacing the `#include` inside `letters_img.h` to point to the new imported texture.

To correct the sizes, you also have to change the `#define` inside `fonts/font.h`.

It will only allow up to 255 ASCII characters, but you can represent them however you want (including sprites).

The current console size (48x31) takes into consideration that each character is 6x6, so you'll have to change that if your characters are larger or smaller.
