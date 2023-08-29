QT6_LIB_DIR=/usr/lib/x86_64-linux-gnu/qt6
# QT6_LIB_DIR=~/Qt/6.x.x/gcc_64

build_cmake() {
    mkdir build
    cd build
    cmake ..
    make -j4
}

apt-get install -y qt6-base-private-dev fcitx-libs-dev extra-cmake-modules libxkbcommon-dev libglu1-mesa-dev

git clone https://github.com/starreeze/fcitx-qt6.git
cd fcitx-qt6
build_cmake
cp qt6/platforminputcontext/libfcitxplatforminputcontextplugin-qt6.so $QT6_LIB_DIR/plugins/platforminputcontexts
cd ../..
build_cmake
