docker run -it --rm -v$(pwd):/n64 n64sdkmod-build-local make clean -C /n64/src
docker run -it --rm -v$(pwd):/n64 n64sdkmod-build-local make -C /n64/src -j