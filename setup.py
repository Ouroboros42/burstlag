from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy

src_root = "src/"
py_root = src_root + "burstlag/"
cpp_root = src_root + "cpp/"

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
        "cpplib", "src/cpp",
        numpy.get_include()
    ]
)