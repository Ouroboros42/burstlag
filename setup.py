from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy

py_lang_level = "3"
c_lang = "c++"

build_root = "build/"

src_root = "src/"
py_root = src_root + "burstlag/"
cpp_root = py_root + "cpp/"

cpp_lib_root = "cpplib/"

source_files = [
    py_root + "__init__.pyx",
    cpp_root + "caching/factorials.cpp",
    cpp_root + "fast_sum/converging.cpp",
    cpp_root + "fast_sum/lazy.cpp",
    cpp_root + "fast_sum/sum_terms.cpp",
    cpp_root + "inputs/relation.cpp"
]

include_dirs = [
    cpp_lib_root,
    cpp_root,
    numpy.get_include()
]

ext = Extension("*", source_files, language=c_lang, extra_compile_args = [ "/std:c++17" ])

cython_module = cythonize(ext, build_dir=build_root, language_level=py_lang_level)

setup(name='burst-lag', ext_modules=cython_module, include_dirs=include_dirs)