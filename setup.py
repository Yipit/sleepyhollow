# -*- coding: utf-8 -*-

from distutils.core import setup, Extension
from dist_helper import BuildExtension


setup(
    name='sleepy-hollow',
    version='1.0',
    ext_modules=[
        Extension("sleepy_hollow",
            sources=["sleepy-hollow.sip", "sleepy-hollow.cpp"],
            include_dirs=['.'])],
    cmdclass={'build_ext': BuildExtension})
