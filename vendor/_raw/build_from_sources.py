
import os
import shutil

INVARIANT = "!!"

valid_arch = {
    "x86",
    "x64",
}

valid_os = {
    "win": {
        "lib_ext": [".lib"],
        "bin_ext": [".dll"],
    }
}

valid_toolchain = {
    "vc": {"os": "win", "toolchain_base": "vc"},
    "vct": {"os": "win", "toolchain_base": "vc"},
    "vc100": {"os": "win", "toolchain_base": "vc"},
    "vc110": {"os": "win", "toolchain_base": "vc"},
    "vc120": {"os": "win", "toolchain_base": "vc"},
}

def perfile_copy(src_dir, dest_dir, filter=None, endswith=None, prefix="\t\t"):
    for f in os.listdir(src_dir):
        f_abs = os.path.join(src_dir, f)
        if ((filter is not None and f not in filter)
            or (endswith is not None and not any([f.endswith(ew) for ew in endswith]))):
            continue
        print("{prefix}Copying {0}".format(f, prefix=prefix))
        if os.path.isfile(f_abs):
            shutil.copy(f_abs, dest_dir)
        else:
            shutil.copytree(f_abs, os.path.join(dest_dir, f))

def parse_tree(dir, base_info, parse_str):
    parse_parts = parse_str.split("/")
    parse_parts_l = len(parse_parts)

    dir = os.path.abspath(dir)
    dir_l = len(dir) + 1

    matches = list()
    for root, dirs, files in os.walk(dir):
        endroot = root[dir_l:].replace('\\', '/')

        root_parts = endroot.split("/")
        if len(root_parts) != parse_parts_l:
            continue

        p_version = None
        p_arch = None
        p_toolchain = None
        p_os = None
        bad = False
        for p, r in zip(parse_parts, root_parts):
            if p.startswith("{") and p.endswith("}"):
                parse_type = p[1:-1]
                if parse_type == "version" and r:
                    p_version = r
                elif parse_type == "toolchain" and r:
                    if r not in valid_toolchain:
                        bad = True; break
                    p_toolchain = r
                elif parse_type == "arch" and r:
                    if r not in valid_arch:
                        bad = True; break
                    p_arch = r
                elif parse_type == "os" and r:
                    if r not in valid_os:
                        bad = True; break
                    p_os = r
                else:
                    bad = True; break
            else:
                if r != p:
                    bad = True; break
        if bad:
            continue

        lib_info = base_info.copy()
        lib_info.version = p_version
        if p_arch:
            lib_info.arch = p_arch
        if p_toolchain:
            lib_info.toolchain = p_toolchain
            lib_info.os = valid_toolchain[p_toolchain]["os"]
        if p_os:
            lib_info.os = p_os
        matches.append((lib_info, root))

    if len(matches) == 0:
        return None, None

    return sorted(matches, key=lambda v: v[0].version, reverse=True)[0]


class TargetInfo(object):
    """Contains target info."""
    def __init__(self, **kwargs):
        self.arch = kwargs.get("arch")
        self.os = kwargs.get("os")
        self.toolchain = kwargs.get("toolchain")

    def __str__(self):
        return "{os}_{arch}_{toolchain}".format(
            **self.__dict__)


class LibInfo(TargetInfo):
    """Contains library info about the specifc variation."""
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.version = kwargs.get("version")
        self.package = kwargs.get("package")

    def copy(self):
        return LibInfo(**self.__dict__)

    def __str__(self):
        return "[{package}:{version}]{0}".format(
            super().__str__(),
            **self.__dict__)

    def __repr__(self):
        return str(self)


class Lib(object):
    """Base class for libraries. Provides accessibility.
    """

    def __init__(self):
        pass

    def headers_only(self):
        """Describes if headers only"""
        return False

    def display_name(self):
        """Long form display name."""
        return self.dir_root()

    def __str__(self):
        return self.display_name()

    def __hash__(self):
        return hash(self.dir_root())

    def __eq__(self, other):
        return other == self.dir_root()

    def prime_variation(self):
        """The variation to use primarily."""
        return "dev"

    def build_variation(self):
        """The variation to use for builds."""
        return self.prime_variation()

    def source_variation(self):
        """The variation to use for source code."""
        return self.prime_variation()

    def dir_root(self):
        """Root directory name."""
        pass

    def check_variations(self, dir):
        """Checks for the primary variation."""
        lib_info, parsed_dir = parse_tree(
            os.path.join(dir, self.dir_root()),
            *self.variations[self.source_variation()])

        print("\tFound Source {0}".format(lib_info))

        self._source_dir = parsed_dir
        self._source_lib_info = lib_info

        lib_info, parsed_dir = parse_tree(
            os.path.join(dir, self.dir_root()),
            *self.variations[self.build_variation()])

        if parsed_dir != self._source_dir:
            print("\tFound Build {0}".format(lib_info))

            self._build_dir = parsed_dir
            self._build_lib_info = lib_info
        else:
            self._build_dir = self._source_dir
            self._build_lib_info = self._source_lib_info

        return self._source_lib_info

    def dir_include(self):
        """The directory name of the includes folder to copy."""
        return (self.dir_root(), False)

    def dir_target_include(self):
        """The directory name of where to copy the includes to."""
        return (self.dir_root(), False)

    def copy_include(self, destination):
        """Copys the include directory (caluclated rlative to a raw_root) to the given destination."""
        print("\tCopying `{0}` Includes.".format(self))

        dest_dir, dest_copy_into = self.dir_target_include()
        dest_dir = os.path.abspath(os.path.join(destination, dest_dir))
        src_dir, src_copy_from = self.dir_include()
        src_dir = os.path.abspath(os.path.join(self._source_dir, src_dir))

        if not dest_copy_into:
            if os.path.exists(dest_dir):
                print("\t\tDeleting '{dest_dir}'".format(
                    dest_dir=dest_dir))
                shutil.rmtree(dest_dir)

        if not src_copy_from and not dest_copy_into:
            print("\t\tCopying tree '{0}' to '{1}'.".format(
                src_dir,
                dest_dir))
            shutil.copytree(
                src_dir,
                dest_dir)
        else:
            print("\t\tCopying each '{0}' to '{1}'.".format(
                src_dir,
                dest_dir))
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)

            perfile_copy(src_dir, dest_dir, prefix="\t\t\t")

    def dir_lib(self, target):
        """The directory name of the lib folder with contents to copy."""
        return "lib"

    def dir_bin(self, target):
        """The directory name of the bin folder with contents to copy."""
        return "bin"

    def filter_base(self, target):
        return None

    def filter_bin(self, target):
        f_base = self.filter_base(target)
        bin_exts = valid_os[target.os]["bin_ext"]
        if f_base is None:
            return (None, bin_exts)
        return ([f_b+ext for f_b in f_base for ext in bin_exts], bin_exts)

    def filter_lib(self, target):
        f_base = self.filter_base(target)
        lib_exts = valid_os[target.os]["lib_ext"]
        if f_base is None:
            return (None, lib_exts)
        return ([f_b+ext for f_b in f_base for ext in lib_exts], lib_exts)

    def copy_lib(self, dest_dir, target):
        src_dir = os.path.abspath(os.path.join(
            self._build_dir,
            self.dir_lib(target)))

        print("\tCopying `{0}` lib from '{1}'.".format(self, src_dir))

        perfile_copy(src_dir, dest_dir, *self.filter_lib(target))

    def copy_bin(self, dest_dir, target):
        src_dir = os.path.abspath(os.path.join(
            self._build_dir,
            self.dir_bin(target)))

        print("\tCopying `{0}` bin from '{1}'.".format(self, src_dir))

        perfile_copy(src_dir, dest_dir, *self.filter_bin(target))


class Lib_Glm(Lib):
    def __init__(self):
        super().__init__()

        self.variations = {
            "dev": (LibInfo(**{
                "arch": INVARIANT,
                "os": INVARIANT,
                "toolchain": INVARIANT,
                "package": "headers"
            }), "{version}"),
        }

    def headers_only(self):
        return True

    def dir_root(self):
        return "glm"


class Lib_Glew(Lib):
    def __init__(self):
        super().__init__()

        self.variations = {
            "dev": (LibInfo(**{
                "arch": INVARIANT,
                "package": "dev"
            }), "dev/{toolchain}/{version}"),
        }

    def dir_root(self):
        return "glew"

    def dir_include(self):
        return ("include/GL", True)

    def dir_target_include(self):
        return ("GL", True)

    def dir_lib(self, target):
        return ("lib/Release/Win32" if target.arch is "x86"
            else "lib/Release/x64")

    def dir_bin(self, target):
        return ("bin/Release/Win32" if target.arch is "x86"
            else "bin/Release/x64")

    def filter_base(self, target):
        return ["glew32"]


class Lib_Sdl2(Lib):
    def __init__(self):
        super().__init__()

        self.variations = {
            "dev": (LibInfo(**{
                "arch": INVARIANT,
                "package": "dev"
            }), "dev/{toolchain}/{version}"),
        }

    def dir_root(self):
        return "sdl"

    def dir_include(self):
        return ("include", False)

    def dir_target_include(self):
        return ("SDL2", True)

    def dir_lib(self, target):
        return os.path.join("lib", target.arch)

    def dir_bin(self, target):
        return os.path.join("lib", target.arch)

    def filter_base(self, target):
        return ["SDL2"]


class Lib_Sdl2_Image(Lib_Sdl2):
    def __init__(self):
        super().__init__()

        self.variations = {
            "dev": (LibInfo(**{
                "arch": INVARIANT,
                "package": "dev"
            }), "dev/{toolchain}/{version}"),
        }

    def dir_root(self):
        return "sdl_image"

    def filter_base(self, target):
        return None


class Lib_AssImp(Lib):
    def __init__(self):
        super().__init__()

        self.variations = {
            "source": (LibInfo(**{
                "arch": INVARIANT,
                "os": INVARIANT,
                "toolchain": INVARIANT,
                "package": "source"
            }), "source/{version}"),
            "build": (LibInfo(**{
                "arch": INVARIANT,
                "package": "build"
            }), "build/{version}/{arch}/{toolchain}")
        }

    def build_variation(self):
        """The variation to use for builds."""
        return "build"

    def source_variation(self):
        """The variation to use for source code."""
        return "source"

    def dir_root(self):
        return "assimp"

    def dir_include(self):
        return ("include/assimp", False)

    def dir_lib(self, target):
        return os.path.join("code", "Release")

    def dir_bin(self, target):
        return os.path.join("code", "Release")

    def filter_base(self, target):
        return ["assimp"]


def main(libs, target):
    root_raw = os.path.abspath(os.path.dirname(__file__))
    root_vendor = os.path.abspath(os.path.join(root_raw, ".."))
    root_lib = os.path.join(root_vendor, "lib", str(target))
    root_bin = os.path.join(root_vendor, "bin", str(target))

    if not os.path.exists(root_lib):
        os.makedirs(root_lib)
    if not os.path.exists(root_bin):
        os.makedirs(root_bin)

    versions = {}
    loaded_versions = {}

    print("Building vendor '{0}' from raw '{1}'.".format(
        root_vendor,
        root_raw))

    for lib in libs:
        print("Running Library `{0}`".format(lib))
        info = lib.check_variations(root_raw)
        if info is None:
            continue

        versions[lib] = info.version
        old_version = loaded_versions.get(lib)
        if old_version == info.version:
            print("\tVersion up-to-date ({0}), skipping!")
            continue

        lib.copy_include(os.path.join(root_vendor, "include"))
        if not lib.headers_only():
            lib.copy_lib(root_lib, target)
            lib.copy_bin(root_bin, target)

if __name__ == "__main__":
    main([
        Lib_Sdl2(),
        Lib_Sdl2_Image(),
        Lib_Glew(),
        Lib_Glm(),
        Lib_AssImp(),
        ], TargetInfo(
            os="win",
            arch="x64",
            toolchain="vc120",
        ))
