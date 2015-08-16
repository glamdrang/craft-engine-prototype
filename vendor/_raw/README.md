This folder is designed to include the raw downloads from various vendor sources and to place them in appropriate folders as needed.

Included in this folder is a script for building the external vendor sources from raw downloads. To use it:

```
python build_from_sources.py
```

Included is the list of expected/supported sources (and their `destination/folders` relative to this one). Below are values for variables:

* `{arch}`: Architecture
    * `x86` (32 bit)
    * `x86_64`

* `{os}`: Operating system
    * `win`
    * `linux`
    * `osx`

* `{toolchain}`: Runtime/Toolchain Expected
    * `vc***` Visual C (Visual Studio), where *** is the specific version (e.g. 100). `t` is a special version (`vct` means msvcrt runtime is used). And `_` means runtime independent (`vc_` means compiled with visual studio but runtime independent).

* `{version}`: Pull this foldername for version of libraries.

# Supported:

* SDL2 ([downloads](https://www.libsdl.org/download-2.0.php)). For windows only, package manager otherwise.
    * Development Libraries `sdl2/dev/{toolchain}/{version}`

* SDL2_image ([downloads](https://www.libsdl.org/projects/SDL_image/)). For windows only, package manager otherwise.
    * Development Libraries `sdl2_image/dev/{toolchain}/{version}`
  
* glew ([downloads](http://glew.sourceforge.net/)). For windows only, package manager otherwise.
    * Development Libraries `glew/dev/{toolchain}/{version}`
  
* assimp ([downloads](http://assimp.sourceforge.net/main_downloads.html))
    * Development Source `assimp/source/{version}` (uses **CMake**)
    * Development Builds `assimp/build/{toolchain}/{version}`
  
* glm ([site](http://glm.g-truc.net/0.9.7/index.html)). Headers only.
    * Source `glm/{version}`