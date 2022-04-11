## jpgtools

本项目是用于Android相机外挂测试的图像工具库. 主要用于解决adb访问和测试图像导致的性能问题, 该项目生成的可执行文件可直接在Android环境下运行. 配合相机测试脚本, 可以实现部分图像检测的功能.

## 编译

因Android ELF的编译依赖NDK, 因此需要在[build.sh](build.sh)中配置本地NDK环境, 默认如下:
```
export ANDROID_NDK=$HOME/android-ndk-r21e
```

编译执行:
```
./build.sh
```
生成的可执行文件将放在项目根目录下, `build.sh`脚本也会将其推送到`/vendor/bin`

## 集成

输出结果以一个空格区分, 以便外部脚本过滤, 已集成的功能如下:

1. `jpgsize <input file>`返回图像的大小, 用于准确的小图检测, 支持管道输入, 如以下返回:
```
( width height pixels size )=( 3072 4096 12582912 2289760 )
```

2. `jpgchannel <input file>`返回图像rgb通道统计, 用于偏色判断, 如紫图/绿图/全黑图/全白图判断, 支持管道输入, 如以下返回:
```
( r g b )=( 34.474852 33.343373 35.255396 )
```

## LICENSE

因该项目主要依赖[jpeg_decoder.h](jpeg_decoder.h), 应此也继承了以下许可[KeyJ's Research License](jpeg_decoder.h)