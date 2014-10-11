#ifndef package_mesh_hpp
#define package_mesh_hpp

#include <boost/python.hpp>
#include <algorithm>
#include <iterator>
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

        hitResult evaluateHit(const vektor & origin, const vektor & direction) {
            boost::python::list temp = intersect(origin, direction);
            std::vector<hitResult> hitResults;
            for(uint32_t i = 0; i < boost::python::len(temp); ++i) {
                hitResults.push_back(boost::python::extract<hitResult>(temp[i]));
            }
            std::vector<hitResult>::iterator theHit = std::min_element(hitResults.begin(), hitResults.end());
            theHit->set_element(std::distance(hitResults.begin(), theHit));
            if(std::count(hitResults.begin(), hitResults.end(), *theHit) != 1) {
                (*theHit).set_hasHit(false);
            }
            return *theHit;
        }

        boost::python::list evaluateHits(const boost::python::list & origins, const boost::python::list & directions) {
            std::vector<vektor> tempOrigins;
            std::vector<vektor> tempDirections;
            uint32_t N = boost::python::len(origins);
            for(uint32_t i = 0; i < N; ++i) {
                tempOrigins.push_back(boost::python::extract<vektor>(origins[i]));
                tempDirections.push_back(boost::python::extract<vektor>(directions[i]));
            }
            std::vector<vektor>::iterator iterOrigins = tempOrigins.begin();
            std::vector<vektor>::iterator iterDirections = tempDirections.begin();
            boost::python::list output;
            hitResult temp;
            for( ; iterOrigins != tempOrigins.end(); ++iterOrigins, ++iterDirections) {
                temp = evaluateHit(*iterOrigins, *iterDirections);
                if(temp.get_hasHit()) {
                    output.append(temp);
                }
            }
            return output;
        }
};

#endif 
