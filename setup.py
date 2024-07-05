from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy

src_root = "src/"
py_root = src_root + "burstlag/"
cpp_root = py_root + "cpp/"

cpp_lib_root = "cpplib"

ext = Extension("*", [
        py_root + "__init__.pyx",
        cpp_root + "fast_sum/lazy.cpp",
        cpp_root + "fast_sum/converging.cpp"
    ],
    language="c++",
    extra_compile_args = [ "/std:c++17" ],
)

cython_module = cythonize(ext, build_dir="build", language_level = "3")

setup(name='burst-lag',
    ext_modules=cython_module,
    include_dirs=[
        cpp_lib_root, cpp_root,
        numpy.get_include()
    ]
)