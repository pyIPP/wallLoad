#ifndef source_wallLoad_cpp
#define source_wallLoad_cpp

#include <boost/python.hpp>
#include <wallLoad.hpp>

class DummyCore{};

using namespace boost::python;
BOOST_PYTHON_MODULE(wallLoad) {
    object coreModule(handle<>(borrowed(PyImport_AddModule("wallLoad.core"))));
    scope().attr("core") = coreModule;
    scope core_scope = coreModule;

    class_<wallLoad::core::vektor>("vektor")
        .def(init<double, double, double>())
        .def(init<wallLoad::core::vektor>())
        .def(init<list>())
        .def(init<dict>())
        .def(init<wallLoad::core::vektor, wallLoad::core::vektor>())
        .add_property("x", &wallLoad::core::vektor::get_x, &wallLoad::core::vektor::set_x)
        .add_property("y", &wallLoad::core::vektor::get_y, &wallLoad::core::vektor::set_y)
        .add_property("z", &wallLoad::core::vektor::get_z, &wallLoad::core::vektor::set_z)
        .add_property("length", &wallLoad::core::vektor::get_length)
        .add_property("normalized", &wallLoad::core::vektor::get_normalized)
        .add_property("normal", &wallLoad::core::vektor::get_normal_vektor)
        .def("dot", &wallLoad::core::vektor::get_dot_product)
        .def("cross", &wallLoad::core::vektor::get_cross_product)
        .def("normalize", &wallLoad::core::vektor::normalize)
        .def("to_list", &wallLoad::core::vektor::to_list)
        .def("to_dict", &wallLoad::core::vektor::to_dict)
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
        .def("__neg__", &wallLoad::core::vektor::operator-)
        .def("angle", &wallLoad::core::vektor::get_angle)
        .def("getRotatedX", &wallLoad::core::vektor::get_rotated_x)
        .def("getRotatedY", &wallLoad::core::vektor::get_rotated_y)
        .def("getRotatedZ", &wallLoad::core::vektor::get_rotated_z)
        .def("distance", &wallLoad::core::vektor::get_distance)
        ;

    class_<wallLoad::core::polygon>("polygon", init<boost::python::list, boost::python::list>())
        .def(init<wallLoad::core::polygon>())
        .def("inside", &wallLoad::core::polygon::inside)
        .add_property("R", &wallLoad::core::polygon::get_R_python)
        .add_property("z", &wallLoad::core::polygon::get_z_python)
        .add_property("size", &wallLoad::core::polygon::size)
        .def("__len__", &wallLoad::core::polygon::size)
        ;



    class_<wallLoad::core::hitResult>("hitResult", init<bool, wallLoad::core::vektor>())
        .def(init<wallLoad::core::hitResult>())
        .add_property("hasHit", &wallLoad::core::hitResult::get_hasHit)
        .add_property("hitPoint", &wallLoad::core::hitResult::get_hitPoint)
        .add_property("element", &wallLoad::core::hitResult::get_element, &wallLoad::core::hitResult::set_element)
        .def("distance", &wallLoad::core::hitResult::get_distance)
        ;      

    class_<wallLoad::core::vertex>("vertex", init<wallLoad::core::vektor, wallLoad::core::vektor, wallLoad::core::vektor>())
        .def(init<wallLoad::core::vertex>())
        .def(init<boost::python::list>())
        .def(init<boost::python::list, boost::python::list, boost::python::list>())
        .add_property("p1", &wallLoad::core::vertex::get_p1, &wallLoad::core::vertex::set_p1)
        .add_property("p2", &wallLoad::core::vertex::get_p2, &wallLoad::core::vertex::set_p2)
        .add_property("p3", &wallLoad::core::vertex::get_p3, &wallLoad::core::vertex::set_p3)
        .add_property("area", &wallLoad::core::vertex::get_area)
        .add_property("normal", &wallLoad::core::vertex::get_normal)
        .add_property("center", &wallLoad::core::vertex::get_center)
        .def("intersect", &wallLoad::core::vertex::intersect)
        ;

    class_<wallLoad::core::probabilityDistribution>("probabilityDistribution", init<boost::python::list, boost::python::list>())
        .def(init<wallLoad::core::probabilityDistribution>())
        .add_property("x", &wallLoad::core::probabilityDistribution::get_x_python)
        .add_property("y", &wallLoad::core::probabilityDistribution::get_y_python)
        .add_property("accumulated", &wallLoad::core::probabilityDistribution::get_accumulated_python)
        .add_property("max", &wallLoad::core::probabilityDistribution::get_max)
        .def("random", &wallLoad::core::probabilityDistribution::get_random_number)
        .def("__call__", &wallLoad::core::probabilityDistribution::get_value)
        ;

    class_<wallLoad::core::radiationProfile>("radiationProfile", init<boost::python::list, boost::python::list>())
        .def(init<wallLoad::core::radiationProfile>())
        .add_property("rho", &wallLoad::core::radiationProfile::get_rho_python)
        .add_property("powerDensity", &wallLoad::core::radiationProfile::get_powerDensity_python)
        .add_property("probabilityDistribution", &wallLoad::core::radiationProfile::get_probabilityDistribution)
        ;

    class_<wallLoad::core::equilibrium>("equilibrium", init<std::string>())
        .def(init<wallLoad::core::equilibrium>())
        .def("psi", &wallLoad::core::equilibrium::get_psi)
        .def("rho", &wallLoad::core::equilibrium::get_rho)
        .add_property("R", &wallLoad::core::equilibrium::get_R_python)
        .add_property("z", &wallLoad::core::equilibrium::get_z_python)
        .add_property("R0", &wallLoad::core::equilibrium::get_R0)
        .add_property("z0", &wallLoad::core::equilibrium::get_z0)
        .add_property("psiAxis", &wallLoad::core::equilibrium::get_psiAxis)
        .add_property("psiEdge", &wallLoad::core::equilibrium::get_psiEdge)
        .add_property("Btor", &wallLoad::core::equilibrium::get_Btor)
        .add_property("Ip", &wallLoad::core::equilibrium::get_Ip)
        .add_property("Rmin", &wallLoad::core::equilibrium::get_Rmin)
        .add_property("Rmax", &wallLoad::core::equilibrium::get_Rmax)
        .add_property("zmin", &wallLoad::core::equilibrium::get_zmin)
        .add_property("zmax", &wallLoad::core::equilibrium::get_zmax)
        ;

    class_<wallLoad::core::radiationDistribution>("radiationDistribution", init<wallLoad::core::equilibrium, wallLoad::core::radiationProfile>())
        .def(init<wallLoad::core::equilibrium, wallLoad::core::radiationProfile, wallLoad::core::polygon>())
        .add_property("Rmin", &wallLoad::core::radiationDistribution::get_Rmin, &wallLoad::core::radiationDistribution::set_Rmin)
        .add_property("Rmax", &wallLoad::core::radiationDistribution::get_Rmax, &wallLoad::core::radiationDistribution::set_Rmax)
        .add_property("zmin", &wallLoad::core::radiationDistribution::get_zmin, &wallLoad::core::radiationDistribution::set_zmin)
        .add_property("zmax", &wallLoad::core::radiationDistribution::get_zmax, &wallLoad::core::radiationDistribution::set_zmax)
        .def("random", &wallLoad::core::radiationDistribution::get_random_points_python)
        .def("randomToroidal", &wallLoad::core::radiationDistribution::get_random_toroidal_points_python)
        ;

    class_<wallLoad::core::mesh>("mesh", init<boost::python::list>())
        .def(init<wallLoad::core::mesh>())
        .def(init<std::string>())
        .def("append", &wallLoad::core::mesh::append)
        .def("evaluateHit", &wallLoad::core::mesh::evaluateHit)
        .def("evaluateHits", &wallLoad::core::mesh::evaluateHits_python)
        .def("__len__", &wallLoad::core::mesh::size)
        .def("__getitem__", &wallLoad::core::mesh::operator[],
            boost::python::return_internal_reference<>())
        ;

    class_<wallLoad::core::radiationLoad>("radiationLoad", init<wallLoad::core::mesh, wallLoad::core::radiationDistribution>())
        .def(init<wallLoad::core::radiationLoad>())
        .def("clear", &wallLoad::core::radiationLoad::clear)
        .def("addSamples", &wallLoad::core::radiationLoad::add_samples)
        .def("__getitem__", &wallLoad::core::radiationLoad::operator[])
        .def("__len__", &wallLoad::core::radiationLoad::size)
        .add_property("size", &wallLoad::core::radiationLoad::size)
        .def("clear", &wallLoad::core::radiationLoad::clear)
        .add_property("totalHits", &wallLoad::core::radiationLoad::get_total_hits)
        .def("getHeatFlux", &wallLoad::core::radiationLoad::get_heat_flux_python)
        .add_property("mesh", &wallLoad::core::radiationLoad::get_mesh)
        ;

    class_<wallLoad::core::diffuseScatter>("diffuseScatter")
        .def("getDirection", &wallLoad::core::diffuseScatter::get_direction)
        ;







}

#endif 
