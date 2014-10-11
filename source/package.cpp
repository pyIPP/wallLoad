#ifndef package_package_cpp
#define package_package_cpp

#include <boost/python.hpp>
//#include <boost/numpy.hpp>
#include <algorithm>
#include "point.hpp"
#include "vektor.hpp"
#include "hitResult.hpp"
#include "vertex.hpp"
#include "mesh.hpp"
#include "directionGenerator.hpp"
#include <probabilityDistribution.hpp>
#include <equilibrium.hpp>
#include <radiationProfile.hpp>
#include <radiationDistribution.hpp>
#include <radiationLoad.hpp>

using namespace boost::python;
BOOST_PYTHON_MODULE(wallLoad)
{
    //boost::numpy::initialize();

    class_<point>("point")
        .def(init<double, double, double>())
        .def(init<point>())
        .def(init<boost::python::list>())
//        .def(init<boost::numpy::ndarray>())
        .add_property("x", &point::get_x, &point::set_x)
        .add_property("y", &point::get_y, &point::set_y)
        .add_property("z", &point::get_z, &point::set_z)
        .def(self - self)
        .def("rotateToroidal", &point::rotate_toroidal)
        ;


    class_<vektor>("vektor")
        .def(init<double, double, double>())
        .def(init<vektor>())
 //       .def(init<boost::numpy::ndarray>())
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
//        .def("asarray", &vektor::asarray)
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

    class_<hitResult>("hitResult", init<vektor, vektor, bool, vektor>())
        .def(init<hitResult>())
        .add_property("hasHit", &hitResult::get_hasHit)
        .add_property("distance", &hitResult::get_distance)
        .add_property("hitPoint", &hitResult::get_hitPoint)
        .add_property("origin", &hitResult::get_origin)
        .add_property("direction", &hitResult::get_direction)
        .add_property("element", &hitResult::get_element, &hitResult::set_element)
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
        .add_property("area", &vertex::get_area)
        ;

    class_<mesh>("mesh")
        .def(init<mesh>())
        .def(init<boost::python::list>())
        .def("append", &mesh::append)
        .def("intersect", &mesh::intersect)
        .def("evaluateHit", &mesh::evaluateHit)
        .def("evaluateHits", &mesh::evaluateHits)
        ;

    class_<directionGenerator>("directionGenerator")
        .def("generate", &directionGenerator::generate)      
        ;

    class_<probabilityDistribution>("probabilityDistribution", init<boost::python::list, boost::python::list>())
        .def(init<probabilityDistribution>())
        .add_property("accumulated", &probabilityDistribution::get_accumulated)
        .add_property("x", &probabilityDistribution::get_x)
        .add_property("y", &probabilityDistribution::get_y)
        .add_property("max", &probabilityDistribution::get_max)
        .def("random", &probabilityDistribution::get_random_number)
        .def("__call__", &probabilityDistribution::get_value)
        ;

    class_<equilibrium>("equilibrium", init<std::string>()) 
        .def(init<equilibrium>())
        .def("__call__", &equilibrium::get_rho)
        .def("getPsi", &equilibrium::get_psi)
        .add_property("comment", &equilibrium::get_comment)
        .add_property("shape", &equilibrium::get_shape)
        .add_property("R", &equilibrium::get_R)
        .add_property("z", &equilibrium::get_z)
        .add_property("R0", &equilibrium::get_R0)
        .add_property("z0", &equilibrium::get_z0)
        .add_property("psiEdge", &equilibrium::get_psiEdge)
        .add_property("psiAxis", &equilibrium::get_psiAxis)
        .add_property("Btor", &equilibrium::get_Btor)
        .add_property("Rmax", &equilibrium::get_Rmax)
        .add_property("Rmin", &equilibrium::get_Rmin)
        .add_property("zmax", &equilibrium::get_zmax)
        .add_property("zmin", &equilibrium::get_zmin)
        ;

    class_<radiationProfile>("radiationProfile", init<boost::python::list, boost::python::list>())
        .def(init<radiationProfile>())
        .add_property("rho", &radiationProfile::get_rho)
        .add_property("powerDensity", &radiationProfile::get_powerDensity)
        .add_property("probabilityDistribution", &radiationProfile::get_probabilityDistribution)
        ;   

    class_<radiationDistribution>("radiationDistribution", init<equilibrium, radiationProfile>())
        .def("__call__", &radiationDistribution::get_random_points)
        .add_property("Rmax", &radiationDistribution::get_Rmax, &radiationDistribution::set_Rmax)
        .add_property("Rmin", &radiationDistribution::get_Rmin, &radiationDistribution::set_Rmin)
        .add_property("zmax", &radiationDistribution::get_zmax, &radiationDistribution::set_zmax)
        .add_property("zmin", &radiationDistribution::get_zmin, &radiationDistribution::set_zmin)
        ;  

    class_<radiationLoad>("radiationLoad", init<mesh, radiationDistribution>())
        .def("getSamples", &radiationLoad::get_samples)
        ;  

}

#endif 
