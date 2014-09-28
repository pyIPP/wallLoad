#ifndef package_package_cpp
#define package_package_cpp

#include <boost/python.hpp>
#include <boost/numpy.hpp>
#include "point.hpp"
#include "vektor.hpp"
#include "hitResult.hpp"
#include "vertex.hpp"
#include "mesh.hpp"
#include "directionGenerator.hpp"

using namespace boost::python;
BOOST_PYTHON_MODULE(wallLoad)
{
    boost::numpy::initialize();

    class_<point>("point")
        .def(init<double, double, double>())
        .def(init<point>())
        .def(init<boost::python::list>())
        .def(init<boost::numpy::ndarray>())
        .add_property("x", &point::get_x, &point::set_x)
        .add_property("y", &point::get_y, &point::set_y)
        .add_property("z", &point::get_z, &point::set_z)
        .def(self - self)
        ;


    class_<vektor>("vektor")
        .def(init<double, double, double>())
        .def(init<vektor>())
        .def(init<boost::numpy::ndarray>())
        .def(init<boost::python::list>())
        .def(init<boost::python::dict>())
        .def(init<point, point>())
        .def(init<point>())
        .add_property("x", &vektor::get_x, &vektor::set_x)
        .add_property("y", &vektor::get_y, &vektor::set_y)
        .add_property("z", &vektor::get_z, &vektor::set_z)
        .add_property("length", &vektor::get_length)
        .add_property("normalized", &vektor::get_normalized)
        .def("dot", &vektor::get_dot_product)
        .def("cross", &vektor::get_cross_product)
        .def("normalize", &vektor::normalize)
        .def("asarray", &vektor::asarray)
        .def("to_list", &vektor::to_list)
        .def("to_dict", &vektor::to_dict)
        .def(self_ns::str(self))
        .def(self += self)
        .def(self + self)
        .def(self -= self)
        .def(self - self)
        .def(self *= double())
        .def(self * double())
        .def(double() * self)
        .def(self /= double())
        .def(self / double())
        .def(self == self)
        .def(self != self)
        .def("__neg__", &vektor::operator-)
        ;

    class_<hitResult>("hitResult")
        .def(init<bool, double, vektor>())
        .def(init<hitResult>())
        .add_property("hasHit", &hitResult::get_hasHit)
        .add_property("distance", &hitResult::get_distance)
        .add_property("hitPoint", &hitResult::get_hitPoint)
        ;      

    class_<vertex>("vertex")
        .def(init<point, point, point>())
        .def(init<vektor, vektor, vektor>())
        .def(init<vertex>())
        .def(init<boost::python::list, boost::python::list, boost::python::list>())
        .def(init<boost::python::list>())
        .add_property("p1", &vertex::get_p1, &vertex::set_p1)
        .add_property("p2", &vertex::get_p2, &vertex::set_p2)
        .add_property("p3", &vertex::get_p3, &vertex::set_p3)
        .def("intersect", &vertex::intersect)
        ;

    class_<mesh>("mesh")
        .def(init<mesh>())
        .def(init<boost::python::list>())
        .def("append", &mesh::append)
        .def("intersect", &mesh::intersect)
        ;

    class_<directionGenerator>("directionGenerator")
        .def("generate", &directionGenerator::generate)      
        ;
}

#endif 
