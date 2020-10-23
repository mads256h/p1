# p1

![CMake Windows](https://github.com/mads256h/p1/workflows/CMake%20Windows/badge.svg)
![CMake MacOS](https://github.com/mads256h/p1/workflows/CMake%20MacOS/badge.svg)
![CMake Ubuntu](https://github.com/mads256h/p1/workflows/CMake%20Ubuntu/badge.svg)

This repository is built with cmake.
It has these dependencies:

```
json-c
```


## Build for Windows

Install [MSYS2](https://msys.org)

Update your MSYS2 installation:

```
pacman -Suy
```

If it asks you to restart MSYS2 do it and run the command again.

Install dependencies:

```
pacman -Suy mingw-w64-x86_64-json-c mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
```

Close the MSYS2 terminal and open the MSYS2 mingw64 terminal. cd into the repository and run:

```
./build_msys.sh
```


## Build for MacOS

Install [Homebrew](https://brew.sh)

Install dependencies:

```
brew install gcc make cmake json-c
```

cd into repository and run:

```
./build_macos.sh
```

## Build for Linux

Install dependencies:

```
gcc make cmake json-c
```

Build:

```
./build_linux.sh
```


### Dependencies for Arch Linux

```
pacman -Suy gcc make cmake json-c
```

