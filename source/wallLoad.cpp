#ifndef source_wallLoad_cpp
#define source_wallLoad_cpp

#include <boost/python.hpp>
#include <wallLoad.hpp>

using namespace boost::python;
BOOST_PYTHON_MODULE(wallLoad) {
    scope core;
    class_<wallLoad::core::vektor>("vektor")
        ;

}

#endif 
