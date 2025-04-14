<p align="center">
  <img width="346" alt="image" src="https://github.com/user-attachments/assets/74fa82a9-638a-44af-962b-45d2b358c22a" />
</p>

<p align="center">High performance C library collection</p>
<p align="center"><img  alt="image" src="https://img.shields.io/badge/version-0.1.0-7446ff" /></p>

## Library

<details>

<summary>Test</summary>

**[`hac_ut`](/hac_ut/README.md)**

</details>

## Installation

`Ha-C` uses `CMake` to build and install libraries.

```shell
# Create output dir
mkdir build && cd build

# Configure CMake
cmake .. \
  -DCMAKE_C_COMPILER=gcc \ # choose a c compiler
  -DCMAKE_INSTALL_PREFIX=/usr \ # target installation directory path prefix
  -DCMAKE_BUILD_TYPE=Release 
  
# Build all libs
cmake --build .

# Build specific shared and static libs
# cmake --build . --target hac_{lib} hac_{lib}_static 

# Install
sudo cmake --install .  # make sure you have the right permission to access the installation path
```

After installation, the header files are in the `/usr/include/hac` while the static and dynamic libraries are in the `/usr/lib/hac`.

## Usage

> The following is sample code usage description:

```c
#include <stdio.h>
#include "sum/sum.h"

int main() {
  printf("%d\n", sum(1, 2));
  return 0;
}
```

## API

> The following is sample code API description:

### `int sum(int a, int b)`

Calculate two `integer`s sum.



## Support

**Most** of the repo's libs work on:

### Compiler

gcc, clang, msvc

### C standard

C11

### OS and arch

- windows(x64, arm64)
- macos(x64, arm64)
- freebsd@14, openbsd@7, netbsd@10(x64, arm64)
- linux(x64, arm64, armv6, armv7, riscv64, 386, s390x)
