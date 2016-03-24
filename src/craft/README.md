Description of source folder contents. Each folder has it's own README.md, in general files will be listed in the order of interest.

### File Naming Convention

* `CamelCase.*` - Describes a class (and it's functions), along with support structures and functions (although rare).
  * `ICamelCase.*` - Describes an "interface" class. Often abstract (or mostly abstract).
* `under_score.*` - Describes a "system" of classes and functions (although mostly stand alone functions with some helper structures).

### File Extensions

* `.h` - Header file (where declarations live).
* `.cpp` - C++ compilation unit (where definitions live).
* `.hpp` - A header file containing inline code (often a templated class).

## Code Standard

While this may not be finished in being applied, this is the official coding standard.

* `ClassesAreCamelCase`
  * `IInterfacesStartWithAnI` - Are also abstract
  * `ClassesThatAreBasesEndWithBase`
    * `GeneralRuleForThingsLikeEvent`
    * `OrOtherComponents`    
* `classMethodsAndVariablesAreCamelCaseBesidesFirst`
  * `_withAnUnderscoreIfPrivate`
  * `camelWithA_underscoreIfProtectedOrInternal`
  * `InterfacesMightUse_anUnderscoreForFunctionsToo`
* `stand_alone_functions_and_variables_are_underscored`
  * `_with_a_single_underscore_for_private_functions`
  * `__with_a_double_underscore_for_private_variables`
* `function_arguments_use_underscores`
* `otherVariablesUseLowerCamelCase_andSometimesUnderscores`
  
## Design

There are 4 layers to this engine, as described below:

* **Environment** (layer 0): The OS and hardware of the client and/or build machine. We mostly ignore this by abstracting it behind a couple of prototypical platform configuraitons.
* **Programming System** (layer 1): The compiler, programming language, utility libraries, and simple third party libraries. These are all the things which we use to build the engine and are shared by the engine and the game. This is basically everything in the `common.h` file (e.g. the util folder, the ecs, and a some special files). Libraries like `format` (from `spdlog`), `glm` (used as our internal vector library), and `yaml-cpp`.
* **Engine Features** (layer 2): This is the bulk of the code in the Engine library. Engine features are implemented using, essentially, a DSL constructed out of other parts. Engine features often wrap larger libraries. Libraries like `GL`, `SDL2` and `AssImp`.
* **Game** (layer 3): This is the place where the engine is used by a user of the system. Custom engine features can still be added of course, but they would be on the boundry between the layers.

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
* `util`: Features combining many subsystems.
