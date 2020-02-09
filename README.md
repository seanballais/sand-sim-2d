# Sand Sim 2D
A demo sand simulator inspired by the 2D sand simulator showcased by Petri Purho in his 2019 GDC Talk, ["Exploring the Tech and Design of Noita"](https://www.youtube.com/watch?v=prXuyMCgbTc).

At the moment, it only supports Linux. Unix-like OSes, such as macOS and FreeBSD, may be supported but it is not guaranteed.

This project is intended to be a quick weekend project, so please do not expect the code to be production-ready.

## Building
### Prerequisites

* C++ compiler that supports C++17 (e.g. GCC 9).
* CMake (`>= 3.1`)
* Conan

### Compilation

    $ cd /path/to/project
    $ mkdir build
    $ cd build
    $ conan install ..
    $ cmake ..

### Running
The binary is stored inside the `bin/` directory inside the `build/` directory. Running the simulator simply involves invoking the program, `sand-sim-2d`.

In the terminal, if you are still in the build directory, it can be via:

    $ ./bin/sand-sim-2d

Otherwise, you have to perform:

    $ cd /path/to/wherever/bin/is
    $ ./sand-sim-2d

## Projects Used
This project uses [SFML](http://sfml-dev.org) and the [Source Code Pro](https://github.com/adobe-fonts/source-code-pro) font from Adobe.
