# Overview
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" 
connectivity protocol. This project implements all the messages from 
the protocol specification [v5](http://docs.oasis-open.org/mqtt/mqtt/v5.0/cs02/mqtt-v5.0-cs02.html) .

The protocol is defined in the [schema](dsl/schema.xml)
file using [CommsDSL](https://github.com/arobenko/CommsDSL-Specification).
The **commsds2comms** code generator from [commsdsl](https://github.com/arobenko/commsdsl)
project is used to generate CMake project with proper C++ classes that
implement protocol itself as well as protocol plugin for the
[CommsChampion Tools](https://github.com/arobenko/comms_champion#commschampion-tools).

The [src](src) folder contains additional and/or overriding default functionality 
code snippets, that get injected into the generated code.

The generated CMake project of this protocol definition is hosted and can be
viewed at [cc.mqtt5.generated](https://github.com/arobenko/cc.mqtt5.generated)

# License
Please read [License](https://github.com/arobenko/commsdsl#license)
section from [commsdsl](https://github.com/arobenko/commsdsl) project.

# How to Build
This project uses CMake as its build system. Please open main
[CMakeLists.txt](CMakeLists.txt) file and review available options as well as
mentioned available parameters, which can be used in addition to standard 
ones provided by CMake itself, to modify the default build. 

The **commsdsl2comms** application from [commsdsl](https://github.com/arobenko/commsdsl)
project is used to generate appropriate C++ code. If path to externally
built **commsdsl2comms** is not provided, then this project will build the
latter itself.

The **commsdsl2comms** application (if built internally) requires 
[Boost](https://www.boost.org) library.
In case Boost libraries are not installed in expected default location
(mostly happens on Windows systems), use variables described in 
[CMake documentation](https://cmake.org/cmake/help/v3.8/module/FindBoost.html) 
to help CMake find required libraries and headers. 
It is recommended to use `-DBoost_USE_STATIC_LIBS=ON` parameter to force
linkage with static Boost libraries.

If code generation output directory is not provided, it defaults to 
`output` subdirectory of the one used to build the project.

### Linux Build
```
$> cd /source/of/this/project
$> mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install .. 
$> make 
```
To build the generated project continue with
```
$> cd output && mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install ..
$> make install
```

### Windows Build
```
$> cd C:\source\of\this\project
$> mkdir build
$> cd build
$> cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release \ 
    -DCMAKE_INSTALL_PREFIX=%cd%/install \
    -DBOOST_ROOT="C:\Libraries\boost_1_65_1" -DBoost_USE_STATIC_LIBS=ON ..
$> nmake 
```
To build the generated project continue with
```
$> cd output 
$> mkdir build 
$> cd build
$> cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release \
    -DCC_QT_DIR=C:\Qt\5.6.3\msvc2015 -DCMAKE_INSTALL_PREFIX=%cd%/install ..
$> nmake install
```
To deploy Qt5 libraries into the installation directory continue with
```
$> nmake deploy_qt
```

# How to Use Generated Code
Please read [How to Use Generated Code](https://github.com/arobenko/commsdsl#how-to-use-generated-code)
guidelines from [commsdsl](https://github.com/arobenko/commsdsl) project.

The [release](https://github.com/arobenko/cc.mqtt5.commsdsl/releases)
artifacts contain doxygen generated documentation of the protocol definition.

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**. I usually
respond within 24 hours.

