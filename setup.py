from setuptools import setup
from Cython.Build import cythonize

anydir = "src/**/"

source_files = [ anydir+"*.pyx", anydir+"*.py" ]

cython_module = cythonize(source_files, build_dir="build", language_level = "3")

setup(name='burst-lag', ext_modules= cython_module)