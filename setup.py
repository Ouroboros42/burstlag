from setuptools import setup
from Cython.Build import cythonize

def anyfile(file_ext: str):
    return f"src/**/*.{file_ext}"

source_files = [ anyfile("py"), anyfile("pyx") ]

cython_module = cythonize(source_files, build_dir="build", language_level = "3")

setup(name='burst-lag', ext_modules=cython_module, include_dirs=["cpplib", "src/cpp"])