[![GitHub (master)](https://img.shields.io/github/checks-status/openscad/openscad/master.svg?logo=github&label=build&logoColor=black&colorA=f9d72c&style=plastic)](https://github.com/openscad/openscad/actions)
[![CircleCI (master)](https://img.shields.io/circleci/project/github/openscad/openscad/master.svg?logo=circleci&logoColor=black&colorA=f9d72c&style=plastic)](https://circleci.com/gh/openscad/openscad/tree/master)
[![Coverity Scan](https://img.shields.io/coverity/scan/2510.svg?colorA=f9d72c&logoColor=black&style=plastic)](https://scan.coverity.com/projects/2510)

[![Visit our IRC channel](https://kiwiirc.com/buttons/irc.libera.chat/openscad.png)](https://kiwiirc.com/client/irc.libera.chat/#openscad)

# About this fork

This is [rarefile](https://github.com/rarefile)'s fork of
[OpenSCAD](https://github.com/openscad/openscad). The `rarefile-edition` branch
is official OpenSCAD plus the extras below. The `master` branch is kept
identical to the official project.

## Dark theme

<img width="3840" height="2115" alt="openscad_10_31_2026%_494" src="https://github.com/user-attachments/assets/1126694d-96ee-4cf7-b883-ba914e5718d5" />

A **Theme** setting that can force a light or dark user interface,
whatever the operating system is set to:

| Choice                    | Result                                                    |
| ------------------------- | --------------------------------------------------------- |
| Follow system (default)   | Same as official OpenSCAD                                 |
| Light                     | Light menus, toolbars, panels and dialogs                 |
| Dark                      | Dark menus, toolbars, panels and dialogs                  |

Choosing Light or Dark also switches the editor and 3D view color schemes to
matching ones, but only while they are still on their defaults. Color schemes
you picked yourself are kept.

**To use it:** Edit → Preferences → Advanced → User Interface → **Theme**.

**Status:** this was proposed to the OpenSCAD project in a pull request and
declined. The maintainers prefer that OpenSCAD follows the operating system's
light/dark setting, which current official builds already do, and are
discussing user-supplied style sheets instead in
[openscad/openscad#7148](https://github.com/openscad/openscad/issues/7148).
Use this fork if you want to force a theme regardless of your system setting.

## Building this fork on Windows

The steps below use [MSYS2](https://www.msys2.org/) and need roughly 10 GB of
disk space. The first build can take an hour; later builds only recompile what
changed.

1. Install MSYS2 and open the **MSYS2 UCRT64** shell.
2. Get the source:

       git clone --recurse-submodules -b rarefile-edition https://github.com/rarefile/openscad.git
       cd openscad

3. Install the build dependencies:

       ./scripts/msys2-install-dependencies.sh qt6

4. Build:

       mkdir build && cd build
       cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DUSE_QT6=ON
       cmake --build .

5. Run `build/openscad.exe` with `C:\msys64\ucrt64\bin` on your `PATH`, for
   example from the UCRT64 shell.

For other platforms, see [Building OpenSCAD](#building-openscad) below; the
steps are the same as for official OpenSCAD.

# What is OpenSCAD?
<p><a href="https://opencollective.com/openscad/donate"><img align="right" src="https://opencollective.com/openscad/donate/button@2x.png?color=white" width="200"/></a>

OpenSCAD is a software for creating solid 3D CAD objects. It is free software and
available for Linux/UNIX, MS Windows and macOS.</p>

Unlike most free software for creating 3D models (such as the famous
application Blender), OpenSCAD focuses on the CAD aspects rather than the
artistic aspects of 3D modeling. Thus this might be the application you are
looking for when you are planning to create 3D models of machine parts but
probably not the tool for creating computer-animated movies.

OpenSCAD is not an interactive modeler. Instead it is more like a
3D-compiler that reads a script file that describes the object and renders
the 3D model from this script file (see examples below). This gives you, the
designer, complete control over the modeling process and enables you to easily
change any step in the modeling process or make designs that are defined by
configurable parameters.

OpenSCAD provides two main modeling techniques: First there is constructive
solid geometry (aka CSG) and second there is extrusion of 2D outlines. As the data
exchange format for these 2D outlines Autocad DXF files are used. In
addition to 2D paths for extrusion it is also possible to read design parameters
from DXF files. Besides DXF files OpenSCAD can read and create 3D models in the
STL and OFF file formats.

# Contents

- [About this fork](#about-this-fork)
- [Getting Started](#getting-started)
- [Documentation](#documentation)
    - [Building OpenSCAD](#building-openscad)
        - [Prerequisites](#prerequisites)
        - [Getting the source code](#getting-the-source-code)
        - [Building for macOS](#building-for-macos)
        - [Building for Linux/BSD](#building-for-linuxbsd)
        - [Building for Linux/BSD on systems with older or missing dependencies](#building-for-linuxbsd-on-systems-with-older-or-missing-dependencies)
        - [Building for Windows](#building-for-windows)
        - [Compilation](#compilation)

# Getting started

You can download the latest binaries of OpenSCAD at
<https://www.openscad.org/downloads.html>. Install binaries as you would any other
software.

When you open OpenSCAD, you'll see three frames within the window. The
left frame is where you'll write code to model 3D objects. The right
frame is where you'll see the 3D rendering of your model.

Let's make a tree! Type the following code into the left frame:

```scad
cylinder(h = 30, r = 8);
```

Then render the 3D model by hitting F5. Now you can see a cylinder for
the trunk in our tree. Now let's add the bushy/leafy part of the tree
represented by a sphere. To do so, we will union a cylinder and a
sphere.

```scad
union() {
    cylinder(h = 30, r = 8);
    sphere(20);
}
```

But, it's not quite right! The bushy/leafy are around the base of the
tree. We need to move the sphere up the z-axis.

```scad
union() {
    cylinder(h = 30, r = 8);
    translate([0, 0, 40]) sphere(20);
}
```

And that's it! You made your first 3D model! There are other primitive
shapes that you can combine with other set operations (union,
intersection, difference) and transformations (rotate, scale,
translate) to make complex models! Check out all the other language
features in the [OpenSCAD
Manual](https://en.wikibooks.org/wiki/OpenSCAD_User_Manual).

# Documentation

Have a look at the OpenSCAD Homepage (https://www.openscad.org/documentation.html) for documentation.

## Building OpenSCAD

To build OpenSCAD from source, follow the instructions for the
platform applicable to you below.

### Prerequisites

To build OpenSCAD, you need some libraries and tools. The version
numbers in brackets specify the versions which have been used for
development. Other versions may or may not work as well.

If you're using a newer version of Ubuntu, you can install these
libraries from aptitude. If you're using Mac, or an older Linux/BSD, there
are build scripts that download and compile the libraries from source.
Follow the instructions for the platform you're compiling on below.

* A C++ compiler supporting C++17
* [cmake (3.5 ->)](https://cmake.org/)
* [Qt (5.15 ->)](https://qt.io/)
* [QScintilla2 (2.9 ->)](https://riverbankcomputing.com/software/qscintilla/)
* [CGAL (5.4 ->)](https://www.cgal.org/)
 * [GMP (5.x)](https://gmplib.org/)
 * [MPFR (3.x)](https://www.mpfr.org/)
* [boost (1.70 ->)](https://www.boost.org/)
* [OpenCSG (1.4.2 ->)](http://www.opencsg.org/)
* [GLEW (1.5.4 ->)](http://glew.sourceforge.net/)
* [Eigen (3.x)](https://eigen.tuxfamily.org/)
* [glib2 (2.x)](https://developer.gnome.org/glib/)
* [fontconfig (2.10 -> )](https://fontconfig.org/)
* [freetype2 (2.4 -> )](https://freetype.org/)
* [harfbuzz (0.9.19 -> )](https://www.freedesktop.org/wiki/Software/HarfBuzz/)
* [libzip (0.10.1 -> )](https://libzip.org/)
* [Bison (2.4 -> )](https://www.gnu.org/software/bison/)
* [Flex (2.5.35 -> )](http://flex.sourceforge.net/)
* [pkg-config (0.26 -> )](https://www.freedesktop.org/wiki/Software/pkg-config/)
* [double-conversion (2.0.1 -> )](https://github.com/google/double-conversion/)

For the test suite, additional requirements are:
* Python3 (3.9 -> )
* [Ghostscript (10.x ->)](https://www.ghostscript.com/index.html)
* [Catch2 (3.0 ->)](https://github.com/catchorg/Catch2)

### Getting the source code

Install git (https://git-scm.com/) onto your system. Then run a clone:

    git clone https://github.com/openscad/openscad.git

This will download the latest sources into a directory named `openscad`.

To pull the various submodules (incl. the [MCAD library](https://github.com/openscad/MCAD)), do the following:

    cd openscad
    git submodule update --init --recursive

### Contributing Changes

You can create an issue to plan and discuss your change by visiting https://github.com/openscad/openscad/issues.

If you want to work on an existing issue and plan to contribute changes via a PR later, you can assign the issue to yourself by commenting:

`/assign-me`

in a comment on the issue.

### Building for macOS

Prerequisites:

* Xcode
* automake, libtool, cmake, pkg-config, wget, meson, python-packaging (we recommend installing these using Homebrew)

Install Dependencies:

After building dependencies using one of the following options, follow the instructions in the *Compilation* section.

1. **From source**

    Run the script that sets up the environment variables:

        source scripts/setenv-macos.sh

    Then run the script to compile all the dependencies:

        ./scripts/macosx-build-dependencies.sh

2. **Homebrew** (assumes [Homebrew](https://brew.sh/) is already installed)

        ./scripts/macosx-build-homebrew.sh

### Building for Linux/BSD

First, make sure that you have git installed (often packaged as 'git-core'
or 'scmgit'). Once you've cloned this git repository, download and install
the dependency packages listed above using your system's package
manager. A convenience script is provided that can help with this
process on some systems:

    sudo ./scripts/uni-get-dependencies.sh qt6

After installing dependencies, check their versions. You can run this
script to help you:

    ./scripts/check-dependencies.sh

Take care that you don't have old local copies anywhere (`/usr/local/`).
If all dependencies are present and of a high enough version, skip ahead
to the Compilation instructions.

### Building for Linux/BSD on systems with older or missing dependencies

If some of your system dependency libraries are missing or old, then you
can download and build newer versions into `$HOME/openscad_deps` by
following this process. First, run the script that sets up the
environment variables.

    source ./scripts/setenv-unibuild.sh

Then run the script to compile all the prerequisite libraries above:

    ./scripts/uni-build-dependencies.sh

Note that huge dependencies like gcc, qt, or glib2 are not included
here, only the smaller ones (boost, CGAL, opencsg, etc). After the
build, again check dependencies.

    ./scripts/check-dependencies.sh

After that, follow the Compilation instructions below.

### Building on Nix

A [development Nix shell](scripts/nix) is included for local, incremental compilation.

### Building for Windows

OpenSCAD for Windows is usually cross-compiled from Linux. If you wish to
attempt an MSVC build on Windows, please see this site:
https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Building_on_Windows

MSVC build support has been added to OpenSCAD. For instructions on how to build it,
refer to [building with MSVC](doc/win-build.md).

To cross-build, first make sure that you have all necessary dependencies
of the MXE project ( listed at https://mxe.cc/#requirements ). Don't install
MXE itself, the scripts below will do that for you under `$HOME/openscad_deps/mxe`

Then get your development tools installed to get GCC. Then after you've
cloned this git repository, start a new clean bash shell and run the
script that sets up the environment variables.

    source ./scripts/setenv-mingw-xbuild.sh 64

Then run the script to download & compile all the prerequisite libraries above:

    ./scripts/mingw-x-build-dependencies.sh 64

Note that this process can take several hours, and tens of gigabytes of
disk space, as it uses the [https://mxe.cc](https://mxe.cc) system to cross-build many
libraries. After it is complete, build OpenSCAD and package it to an
installer:

    ./scripts/release-common.sh mingw64

For a 32-bit Windows cross-build, replace 64 with 32 in the above instructions.

### Building for WebAssembly

We support building OpenSCAD headless for WebAssembly w/ Emscripten, using a premade Docker image built in [openscad/openscad-wasm](https://github.com/openscad/openscad-wasm) (which also has usage examples)

#### Browser

The following command creates `build-web/openscad.wasm` & `build-web/openscad.js`:

```bash
./scripts/wasm-base-docker-run.sh emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Debug -DEXPERIMENTAL=1
./scripts/wasm-base-docker-run.sh cmake --build build-web -j2
```

[openscad/openscad-playground](https://github.com/openscad/openscad-playground) uses this WASM build to provide a [Web UI](https://ochafik.com/openscad2/) with a subset of features of OpenSCAD.

> [!NOTE]
> With a debug build (`-DCMAKE_BUILD_TYPE=Debug`), you can set C++ breakpoints in Firefox and in Chrome (the latter [needs an extension](https://developer.chrome.com/docs/devtools/wasm)).

#### Standalone node.js build

The following command creates `build-node/openscad.js`, which is executable (requires `node`):

```bash
./scripts/wasm-base-docker-run.sh emcmake cmake -B build-node -DWASM_BUILD_TYPE=node -DCMAKE_BUILD_TYPE=Debug -DEXPERIMENTAL=1
./scripts/wasm-base-docker-run.sh cmake --build build-node -j2
build-node/openscad.js --help
```

> [!NOTE]
> With a debug build (`-DCMAKE_BUILD_TYPE=Debug`), you can set C++ breakpoints in VSCode + Node ([needs an extension](https://code.visualstudio.com/docs/nodejs/nodejs-debugging#_debugging-webassembly)).

#### node.js module build

The following command creates `build-node/openscad.js`, which is a node module that can be imported into other node programs:

```bash
./scripts/wasm-base-docker-run.sh emcmake cmake -B build-node -DWASM_BUILD_TYPE=node-module -DCMAKE_BUILD_TYPE=Debug -DEXPERIMENTAL=1
./scripts/wasm-base-docker-run.sh cmake --build build-node -j2
```

Example usage:

```js
import OpenSCAD from "./build-node/openscad.js";

const instance = await OpenSCAD({
    noInitialRun: true,
    print: (text) => {
        console.debug('stdout: ' + text);
    },
    printErr: (text) => {
        console.debug('stderr: ' + text);
    },
});

const inFile = "model.scad";
const outFile = "model.stl";

instance.callMain([
    inFile,
    "-o", outFile,
    "--backend=manifold",
    "--export-format=binstl",
]);

// Model is now generated
```

### Compilation

First, run `cmake -B build -DEXPERIMENTAL=1` to generate a Makefile in the `build` folder.

Then run `cmake --build build`. Finally, on Linux you might run `cmake --install build` as root.

If you had problems compiling from source, raise a new issue in the
[issue tracker on the github page](https://github.com/openscad/openscad/issues).

This site and it's subpages can also be helpful:
https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Building_OpenSCAD_from_Sources

Once built, you can run tests with `ctest` from the `build` directory.

Note: Both `cmake --build` and `ctest` accepts a `-j N` argument for distributing the load over `N` parallel processes.

### Running CI workflows locally

*   Install [circleci-cli](https://circleci.com/docs/2.0/local-cli/) (you'll need an API key)

    *Note*: we also use GitHub Workflows, but only to run tests on Windows (which we cross-build for in the Linux-based CircleCI workflows below). Also, [act](https://github.com/nektos/act) doesn't like our submodule setup anyway.

*   Run the CI jobs

	```bash
	# When "successful", these will fail to upload at the very end of the workflow.
	circleci local execute --job  openscad-mxe-64bit
	circleci local execute --job  openscad-mxe-32bit
	circleci local execute --job  openscad-appimage-64bit
	```

	*Note*: openscad-macos can't be built locally.

*   If/when GCC gets randomly killed, give docker more RAM (e.g. 4GB per concurrent image you plan to run)

*   To debug the jobs more interactively, you can go the manual route (inspect .circleci/config.yml to get the actual docker image you need)

	```bash
	docker run --entrypoint=/bin/bash -it openscad/mxe-x86_64-gui:latest
	```

	Then once you get the console:

	```bash
	git clone https://github.com/%your username%/openscad.git workspace
	cd workspace
	git checkout %your branch%
	git submodule init
	git submodule update

	# Then execute the commands from .circleci/config.yml:
	#    export NUMCPU=2
	#    ...
	#    ./scripts/release-common.sh -snapshot -mingw64 -v "$OPENSCAD_VERSION"
	```
