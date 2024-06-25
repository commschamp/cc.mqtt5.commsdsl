# Overview
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" 
connectivity protocol. This project implements all the messages from 
the protocol specification [v5](http://docs.oasis-open.org/mqtt/mqtt/v5.0/cs02/mqtt-v5.0-cs02.html) .

The protocol is defined in the [schema](dsl/schema.xml)
file using [CommsDSL](https://github.com/commschamp/CommsDSL-Specification).
The **commsds2comms** code generator from [commsdsl](https://github.com/commschamp/commsdsl)
project is used to generate C++11 code of the protocol implementation.

The [src](src) folder contains additional and/or overriding default functionality 
code snippets, that get injected into the generated code.

The generated CMake project of this protocol definition is hosted and can be
viewed at [cc.mqtt5.generated](https://github.com/commschamp/cc.mqtt5.generated)

# License
Please read [License](https://github.com/commschamp/commsdsl#license)
section from [commsdsl](https://github.com/commschamp/commsdsl) project.

# How to Build
This project uses CMake as its build system. Please open main
[CMakeLists.txt](CMakeLists.txt) file and review available options as well as
mentioned available parameters, which can be used in addition to standard 
ones provided by CMake itself, to modify the default build. 

This project also has external dependencies, it requires an access to
the [COMMS Library](https://github.com/commschamp/comms) and
code generators from [commsdsl](https://github.com/commschamp/commsdsl) projects.
These dependencies are expected to be built independenty and access to them provided
via standard **CMAKE_PREFIX_PATH** and/or **CMAKE_PROGRAM_PATH** (for the binaries of
the code generators). There are also scripts (
[script/prepare_externals.sh](script/prepare_externals.sh) for Linux and
[script/prepare_externals.bat](script/prepare_externals.bat) for Windows)
which can help in preparation of these dependencies. They are also used
in configuration of the [github actions](.github/workflows/actions_build.yml) and
[appveyor](.appveyor.yml).

The project's cmake configuration [options](CMakeLists.txt) allow building
bindings to other high level programming languages using [swig](https://www.swig.org/)
and [emscripten](https://emscripten.org/), see relevant commsdsl's
[documentation](https://github.com/commschamp/commsdsl/tree/master/doc) pages for details.

### Linux Build
```
$> cd /source/of/this/project
$> mkdir build && cd build
$> BUILD_DIR=$PWD CC=gcc CXX=g++ COMMON_INSTALL_DIR=$PWD/install COMMON_BUILD_TYPE=Release ../script/prepare_externals.sh
$> cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_PREFIX_PATH=$PWD/install
$> make install
```

### Windows Build
```
$> cd C:\source\of\this\project
$> mkdir build && cd build
$> set BUILD_DIR=%cd%
$> set GENERATOR="NMake Makefiles"
$> set QTDIR=C:\Qt\5.15.2
$> set COMMON_INSTALL_DIR=%cd%/install
$> ..\script\prepare_externals.bat
$> cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=%cd%/install -DCMAKE_PREFIX_PATH=%cd%\install 
$> nmake install
```

# Supported Compilers
Please read [Supported Compilers](https://github.com/commschamp/commsdsl#supported-compilers)
info from [commsdsl](https://github.com/commschamp/commsdsl) project.

# How to Build and Use Generated Code
Please read the
[Generated CMake Project Walkthrough](https://github.com/commschamp/commsdsl/blob/master/doc/GeneratedProjectWalkthrough.md)
documentation page for details on the generated project internals.

The [release](https://github.com/commschamp/cc.mqtt5.commsdsl/releases)
artifacts contain doxygen generated documentation of the protocol definition.

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**. I usually
respond within 24 hours.

