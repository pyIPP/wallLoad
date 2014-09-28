#!/usr/bin/env python
 
from distutils.core import setup
from distutils.extension import Extension
  
setup(name="wallLoad",
    ext_modules=    [
        Extension("wallLoad", ["package.cpp"], 
            library_dirs= ["/usr/local/lib"], 
            libraries = ["boost_python", "boost_numpy"])
                    ]
    )

