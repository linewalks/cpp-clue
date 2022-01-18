export MY_INSTALL_DIR=$HOME/.local
mkdir -p cmake/build
pushd cmake/build
cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../../
make -j 16
popd
