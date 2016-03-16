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
