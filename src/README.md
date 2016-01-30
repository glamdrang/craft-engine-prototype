Description of source folder contents. Each folder has it's own README.md, in general files will be listed in the order of interest.

Naming convention:

* `CamelCase.*` - Describes a class (and it's functions), along with support structures and functions (although rare).
  * `ICamelCase.*` - Describes an "interface" class. Often abstract (or mostly abstract).
* `under_score.*` - Describes a "system" of classes and functions (although mostly stand alone functions with some helper structures).

File types:

* `.h` - Header file (where declarations live).
* `.cpp` - C++ compilation unit (where definitions live).
* `.hpp` - A header file containing inline code (often a templated class).

## Design

We use defines (from the config file) to determine which app gets built and hence fill the `new_App` and `del_App` functions. The driver provides various `main` functions (for example the graphical windows `main`).

## Files

* `config.h`: System configuration options.
* `driver.cpp`: Initalizes (and shutdowns) libraries and wraps `main` function features.
* `core.h`: Drescribes app interface, includes subsystems.
* `common.h`: Includes libraries and common data structures and helpers.

## Folders

* `apps`: Various demonstration apps.
* `gfx`: Graphics; asset loading, scene system, opengl rendering.
* `input`: SDL interaction, controller support, system console.
* `features`: Features combining many subsystems.
