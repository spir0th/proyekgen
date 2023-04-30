<img height="150" src="docs/artwork/logo-sub.png#gh-dark-mode-only" />

<img height="150" src="docs/artwork/logo-light-sub.png#gh-light-mode-only" />

A simple and easy project generator for Windows, Mac, and Linux.

# Table of Contents
- [Introduction](#introduction)
  - [How does it work?](#how-does-it-work)
  - [Usage](#usage)
    - [Specifying output directory](#specifying-output-directory)
    - [List installed templates](#list-installed-templates)
- [Building](#building)
  - [Configurations](#build-configurations)
  - [Prerequisites](#prerequisites)
  - [Compiling](#compiling)
  - [Packaging (optional)](#packaging-optional)
  - [Building on Termux (optional)](#building-on-termux-optional)
- [Contributing](#contributing)
  - [Creating an issue](#creating-an-issue)
  - [Submitting pull requests](#submitting-pull-requests)
- [License](#license)


# Introduction
If you are trying to slack off on work, proyekgen might be your option! With this program, it gets the job done, creating the source files and everything.

## How does it work?
Basically, you'll tell proyekgen to generate a project by using a format.

The *format* is called a *template*, it's a set of project files, post-execute scripts, and information for the kind of project you want.

Templates are also extensible and manageable, meaning you can import and export them in the following paths:

| Windows                             | Unix                                     |
|-------------------------------------|------------------------------------------|
| `%AppData%\proyekgen\templates`     | `$HOME/.local/share/proyekgen/templates` |
| `%ProgramData%\proyekgen\templates` | `/var/lib/proyekgen/templates`           |

*Note for UNIX users: `$HOME/.proyekgen/templates` wll be used instead of `$HOME/.local/share/proyekgen/templates`*
*if XDG dir utilities are not installed*

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

## Building
### Configurations

|                    | Debug               | Release               |
|--------------------|---------------------|-----------------------|
| **Windows 64-bit** | `windows-x64-debug` | `windows-x64-release` |
| **Windows 32-bit** | `windows-x86-debug` | `windows-x86-release` |
| **Linux 64-bit**   | `linux-x64-debug`   | `linux-x64-release`   |
| **Linux 32-bit**   | `linux-x86-debug`   | `linux-x86-release`   |
| **Linux armv7***   | `linux-armv7-debug` | `linux-armv7-release` |

If you don't use these configurations, build/packaging errors might show up unexpectedly.

Notes:

- 1. The build configuration `Linux armv7` is used for building proyekgen inside
[Termux](https://termux.dev), see [Building on Termux](#building-on-termux-optional) section.

### Prerequisites
- Must have a C++ compiler installed (e.g MSVC, GCC, Clang)
- Must have CMake installed (version 3.21 and up)
- Must have the following libraries installed/or built:
  - cxxopts
  - nlohmann-json
  - libarchive
  - libconfig
  - fmt

If you are building on Windows, you can install the libraries by using [Conan](https://conan.io/)
or [vcpkg](https://vcpkg.io)

### Compiling
- Configure to make cache first:

```shell
$ cd <path/to/proyekgen>
$ cmake -S . -B build/<configuration>
```

- Then build to generate executable:

```shell
$ cmake --build build/<configuration>
```

For more info on `<configuration>`, see the [Configurations](#configurations) table.

### Packaging (optional)
proyekgen uses CPack to package itself and integrates well with CMake. Before proceeding to package,
make sure you have the project configured and built the executable.

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

*Note: Some CPack generators (like NSIS and WiX) might be unavailable for source packages.*

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

## Contributing
### Creating an issue
**TODO**

### Submitting pull requests
**TODO**

## License
proyekgen is licensed under the GNU Public License v3, feel free to use it in other programs.