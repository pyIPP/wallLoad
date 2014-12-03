#!/usr/bin/env python
 
from distutils.core import setup
from distutils.extension import Extension
from os import environ

environ['CC'] = 'gcc'
  
setup(name="wallLoad",
    ext_modules=    [
        Extension("wallLoad", ["source/wallLoad.cpp"], 
            include_dirs=['./include'],
            libraries = ["boost_python"],
            extra_compile_args = ["-std=c++11","-w"]
            )
                    ]
    )

