
[![Build Status](https://travis-ci.org/WhoBrokeTheBuild/Noon.svg?branch=master)](https://travis-ci.org/WhoBrokeTheBuild/Noon)

# Noon

Feature testing ground for Dusk.

## Building

You must have the development packages for SFML and Python 3.8+ installed on your
system in order to build Noon.

### Linux

```sh
# Debian/Ubuntu
sudo apt install -y libsfml-dev libpython3.8-dev

# Fedora/RedHat
sudo dnf install -y SFML-devel python38
```

**Note:** Ubuntu doesn't install SFML anywhere CMake can find it. In order to fix
this, you must add it to the `CMAKE_MODULE_PATH` like so.

```sh
export CMAKE_MODULE_PATH=/usr/share/SFML/cmake/Modules/
```

Once that is done, you can build it:

```sh
mkdir build
cd build
cmake ..
make -j8 all run
```

### Windows

Download the latest SFML release from [here](https://www.sfml-dev.org/download.php).

Extract it somewhere and copy the full path, e.g. `C:\Path\To\SFML\`.

When configuring CMake, add `-DSFML_ROOT="C:\Path\To\SFML"` to the arguments.

Once that is done, you can build it:

```powershell
mkdir build
cd build
cmake -DSFML_ROOT="C:\Path\To\SFML" -G "Visual Studio 15 2017 Win64" ..
cmake --build . --target run
```