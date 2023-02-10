# proyekgen
A simple and easy project generator made in C++17.

## Usage
Generate a project by using a template:

```shell
$ proyekgen cmake-cpp
```

### Specifying output directory
By default, proyekgen generates the project inside the current directory.
To use a different directory, pass the `-o` or `--output` option through command-line arguments:

```shell
$ proyekgen <TEMPLATE> -o mydir
$ proyekgen <TEMPLATE> --output mydir # alternative option
```

*Note: proyekgen will result in a fatal error if*
*the directory doesn't exist. Pass the `-m` or `--mkdir` option to create it.*

### List installed templates
You can get the list of installed templates using the `-l` option:

```shell
$ proyekgen -l
There are 5 templates currently installed:
	cmake-c (CMake with C project)
	cmake-cpp (CMake with C++ project)
	makefile-c (Makefile with C project)
	makefile-cpp (Makefile with C++ project)
	python (Simple Python project)
```

or user-specific installed templates:

```shell
$ proyekgen -l --user
There are 1 templates currently installed:
	python (Simple Python project)
```

Alternatively, you can also look for them by navigating the following paths:

| Windows                             | Unix                                     |
|-------------------------------------|------------------------------------------|
| `%AppData%\proyekgen\templates`     | `$HOME/.local/share/proyekgen/templates` |
| `%ProgramData%\proyekgen\templates` | `/var/lib/proyekgen/templates`           |

*Note: `$HOME/.proyekgen/templates` is used instead of `$HOME/.local/share/proyekgen/templates`*
*if XDG dir utilities are not installed*

## Building
### Prerequisites
- Must have a C++ compiler installed (e.g MSVC, GCC, Clang)
- Must have CMake installed (version 3.21 and up)
- Must have the following libraries installed/or built:
  - cxxopts
  - nlohmann-json
  - libarchive
  - libconfig
  - fmt

On Windows, you can install the libraries by using [Conan](https://conan.io/)
or [vcpkg](https://vcpkg.io)

### Compiling
- Configure to make cache first:

```shell
$ cd <path/to/proyekgen>
$ cmake -S . -B build/<platform-arch-type>
```

- Then build to generate executable:

```shell
$ cmake --build build/<platform-arch-type>
```

#### Build configurations
`<platform-arch-type>` can be one of the build configurations in the following table below:

|                    | Debug               | Release               |
|--------------------|---------------------|-----------------------|
| **Windows 64-bit** | `windows-x64-debug` | `windows-x64-release` |
| **Linux 64-bit**   | `linux-x64-debug`   | `linux-x64-release`   |
| **Linux armv7***   | `linux-arm32-debug` | `linux-arm32-release` |
| **macOS 64-bit**   | `macos-x64-debug`   | `macos-x64-release`   |
| **Windows 32-bit** | `windows-x86-debug` | `windows-x86-release` |
| **Linux 32-bit**   | `linux-x86-debug`   | `linux-x86-release`   |
| **macOS 32-bit**   | `macos-x86-debug`   | `macos-x86-release`   |

Notes:

- 1. The build configuration `Linux armv7` is used for building proyekgen inside
[Termux](https://termux.dev), see [Building on Termux](#building-on-termux-optional) section.

### Packaging (optional)
proyekgen uses CPack to package itself and integrates well with CMake. Before proceeding to package,
make sure you have it configured and compiled one.

A basic example of packaging the binary would be:
```shell
$ cd <path/to/build>
$ cpack --config CPackConfig.cmake
```

The command above calls `cpack`, then packages the binary for the current platform.

You can also specify a different or multiple generators:
```shell
$ cpack --config CPackConfig.cmake -G NSIS # NSIS installer only
$ cpack --config CPackConfig.cmake -G ZIP # ZIP file only
$ cpack --config CPackConfig.cmake -G "RPM;DEB" # RPM and DEB files
```

Or, if you want to package the source code and not the binary:
```shell
$ cpack --config CPackSourceConfig.cmake
```

*Note: Some generators (like NSIS and WiX) might be unavailable for source packages.*

### Building on Termux (optional)
- Install the `proot-distro` package and install a distribution:

```shell
$ pkg install proot-distro
...
$ proot-distro install arch
```

*Note: `arch` can be replaced with another distribution, pass the `list`*
*command to get list of available distributions.*

- Enter the PRooted environment and install packages:

```shell
$ proot-distro login arch
...
# Logged inside Arch Linux
...
$ pacman -Syu # optional
$ pacman -S base-devel git cmake cxxopts nlohmann-json libarchive libconfig fmt
```

Notes:

- 1. The package names may vary from the distribution you chose, refer to the documentation for
finding the equivalent names.
  2. The distribution you're using might not provide some of the libraries. Use Arch Linux instead.

- Clone the repository:

```shell
$ git clone https://github.com/spirothdev/proyekgen.git
```

- Follow [build instructions](#compiling) to compile.

## Documentation
proyekgen has not been documented yet, it will be done in the future.

## License
proyekgen is licensed under the GNU Public License v3, feel free to use it in other programs.