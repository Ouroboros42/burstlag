from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

from Cython.Build import cythonize

import numpy

py_lang_level = "3"
c_lang = "c++"
c_std_ver = "c++20"

build_root = "build/"

src_root = "src/"
package_root = src_root + "burstlag/"
cpp_root = package_root + "cpp/"

# 3rd party c++ libraries (header-only)
cpp_lib_root = "cpplib/" 

cpp_source_files = [
    "caching/factorials.cpp",
    "caching/outputs.cpp",
    "fast_sum/sum_terms.cpp",
    "inputs/relation.cpp",
    "util/quadratic.cpp",
]

cython_source_files = [
    "interface.pyx",
]

source_files = [
    *(package_root + src_file for src_file in cython_source_files),
    *(cpp_root + src_file for src_file in cpp_source_files)
]

# Locations with header files
include_dirs = [
    cpp_lib_root,
    cpp_root,
    numpy.get_include()
]

# Match compiler-dependent argument format
platform_extra_compile_args = {
    "msvc": [ f"/std:{c_std_ver}" ],
    "unix": [ f"-std={c_std_ver}"]
}

class PlatformSpecificBuildExt(build_ext):
    def build_extensions(self) -> None:
        extra_compile_args = platform_extra_compile_args.get(self.compiler.compiler_type, [])

        for ext in self.extensions:
            ext.extra_compile_args = extra_compile_args

        return super().build_extensions()

# Object representing extension module
extension = Extension("*", source_files, language=c_lang)
cython_module = cythonize(extension, build_dir=build_root, language_level=py_lang_level)

# Build package
setup(name='burst-lag', ext_modules=cython_module, include_dirs=include_dirs, cmdclass={"build_ext": PlatformSpecificBuildExt})