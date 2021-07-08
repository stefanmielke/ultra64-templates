# First Person Template

This template has:
- First person camera
- Movement (as in Goldeneye/Perfect Dark)
- Simple wall rendering macros

## How to Build

You can run `build.sh` and it will rebuild the whole project everytime.

If you are using VSCode, this project comes with a pre-configured task that will run the bash file when you press `CTRL+SHIFT+B`.

## Project Walkthrough

This project started from the N64SDK sample "ground" and does **not** use NuSystem.

Source code and Makefile are in the `src/` folder.

Inside you have the `.asm` files inside the `asm/` folder, `data` files (files that will be compiled but are not linked, but are used by the `ld` file) inside the `data/` folder, and the `walls/` folder has the wall rendering helpers.

The main file is `game.c`. In it you will find the start and main loop inside the function `void game(void)`. Most of the code is, today, inside that function, but it has some comments to let you know what part does what.