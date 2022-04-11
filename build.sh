function build() {
    export ANDROID_NDK=$HOME/android-ndk-r21e
    export PATH=$ANDROID_NDK:$PATH

    rm -r .build
    mkdir .build && cd .build

    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=arm64-v8a \
        -DANDROID_NDK=$ANDROID_NDK \
        -DANDROID_PLATFORM=latest \
        -DANDROID_LD=lld \
        ..

    make -j12 && make

    cd ..
    rm -r .build/
    ls | xargs -I {} file {} | rg ELF | awk -F':' '{print $1}' | xargs -I {} adb push {} /vendor/bin
}
build
