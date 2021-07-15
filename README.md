# Ultra64 Templates
Game templates for Nintendo 64 games made with [n64sdkmod](https://github.com/CrashOveride95/n64sdkmod)

Each folder is a template you can use to quick-start your game. To install, you can clone the repo (use `--recursive` to get all the files) and copy only the folder you want, or download the repo and do the same.

Most of the code will work with the standard n64sdk, but you will have to create the SPEC file manually based off of the `.ld` file included in the templates.

## Building Templates

All templates include instructions on their respective README files, but all of them include a `build.sh` file in case you have the sdk installed locally, and a `docker-prepare.sh` and `docker-build.sh` for when you want to use Docker instead.
