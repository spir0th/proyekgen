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
There are 1 template currently installed:
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
- Must have a C++ compiler installed (e.g MSVC, GCC, Clang)'
- Must have CMake installed
- Must have the following libraries installed/or built:
  - cxxopts
  - nlohmann-json
  - libarchive
  - libconfig
  - spdlog

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

### Deployment (optional)
There are multiple ways to deploy proyekgen for each platform:

- Windows
  - [EXE installer]()
  - [ZIP file]()
- UNIX
  - [Shell script]()
  - [tar.xz file]()

You can also use: [Automatic Deployment Script]()

*Note: If you plan to deploy your build, make sure you are following*
*the correct [build configurations](#build-configurations) for compilation.*

*Note: Build configurations that start with `-release` are only deployed.*

#### Windows
##### EXE installer
*Note: You must have Inno Setup installed*

Simply go to the `deployment` folder, right click `setup-x64.iss` and
select `Compile` to deploy proyekgen in an EXE installer.

If the deployment fails, then you most likely forgot to compile proyekgen
using the `-release` build configurations.

##### ZIP file
*Documentation not provided, no implementation.*

#### UNIX
##### Shell script
*Documentation not provided, no implementation.*

##### tar.xz file
*Documentation not provided, no implementation.*

#### Automatic Deployment Script
Automatic Deployment Script (or called `deploy.py`) is a Python script
that allows you to deploy proyekgen without problems.

*Note: You must have Python 3 installed to use this script*

##### Running the script
The command below will deploy proyekgen in the current platform and
architecture:

```shell
$ ./deploy.py
```

##### Deploy for specific platform
- Windows builds:

```shell
$ ./deploy.py --platform windows
```

- Linux builds:

```shell
$ ./deploy.py --platform linux
```

##### Deploy for specific architecture
- 64-bit builds:

```shell
$ ./deploy.py --arch x64
```

- 32-bit builds:

```shell
$ ./deploy.py --arch x86
```

- ARMv7 builds:

```shell
$ ./deploy.py --arch arm32
```

*Note: If your machine/OS is 32-bit, and you're deploying for 64-bit,*
*the script will forcely use 32-bit build.*

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
$ pacman -S base-devel git cmake cxxopts nlohmann-json libarchive libconfig spdlog
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