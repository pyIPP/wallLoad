#ifndef package_mesh_hpp
#define package_mesh_hpp

#include <boost/python.hpp>
#include <boost/numpy.hpp>
#include <vector>
#include <stdint.h>
#include "vertex.hpp"
#include "vektor.hpp"


class mesh : std::vector<vertex>
{
    public:
        mesh() : std::vector<vertex>() {}
        mesh(const mesh & rhs) : std::vector<vertex>(rhs) {}
        mesh(const boost::python::list & rhs) : std::vector<vertex>() {
            for(uint32_t i = 0; i < boost::python::len(rhs); ++i){
                std::vector<vertex>::push_back(boost::python::extract<vertex>(rhs[i]));
            }
        }


        virtual ~mesh() {}
        inline void append(const vertex & rhs) {
            std::vector<vertex>::push_back(rhs);
        }
        boost::python::list intersect(const vektor & origin, const vektor & direction) {
            boost::python::list list;
            for(mesh::const_iterator iter = std::vector<vertex>::begin(); iter != std::vector<vertex>::end(); ++iter) {
                list.append(iter->intersect(origin, direction));
            }
            return list;
        }

};

#endif 
