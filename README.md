A starter kit for opengl4 using SDL2 (+Image), AssImp, GLM and GLEW.

This is meant as a quick and dirty overview of how OpenGL is used. It will touch on important concepts and their practical usage. Block comments of the following format are meant as a tutorial through the code. (Start with core.h).

```
/* T:
Some content

about the below code.
*/
```

Other terse development comments also exist.

## Orginization

* `assets`: Contains 3d assets.
* `vendor`: Contains external vendor libraries.
    * `_raw`: Staging area for building the vendor folder from raw development libraries (see README in folder).
* `src`: Contains source code.
* `OpenGL4Starter`: Visual Studio project and build directories.

## Libraries

* **SDL2**: Used to simplify operating specific functions (reading files, creating opengl contexts, event loops).
    * **SDL2_Image**: Used to simplify loading 2d images, and manage the libraries to do so under one API.
* **AssImp**: Used in place of custom file formats and custom parsers for standard ones. Be aware, however, that this library is much slower at loading files due to it's very general nature.
* **GLM**: Vector math library which mimics GLSL functions.
* **GLEW**: Simplifies OpenGL usage in C++.