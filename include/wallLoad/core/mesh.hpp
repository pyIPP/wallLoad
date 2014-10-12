#ifndef include_wallLoad_core_mesh_hpp
#define include_wallLoad_core_mesh_hpp

#include <boost/python.hpp>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdint.h>
#include <include/wallLoad/core/vertex.hpp>
#include <include/wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        class mesh : public std::vector<vertex>
        {
            public:
                mesh(const mesh & rhs) : std::vector<vertex>(rhs) {}
                mesh(const boost::python::list & rhs) : std::vector<vertex>() {
                    for(uint32_t i = 0; i < boost::python::len(rhs); ++i){
                        std::vector<vertex>::push_back(boost::python::extract<vertex>(rhs[i]));
                    }
                }
                virtual ~mesh() {}

                mesh & operator=(const mesh & rhs) const {
                    if(this != &rhs) {
                        std::vector<vertex>::operator=(rhs);
                    }
                    return *this;
                }

                inline void append(const vertex & rhs) {
                    std::vector<vertex>::push_back(rhs);
                }

                std::vector<hitResult> intersect(const vektor & origin, const vektor & direction) const {
                    hitResult temp;
                    std::vector<hitResult> output;
                    for(mesh::const_iterator iter = begin(); iter != end(); ++iter) {
                        temp = iter->intersect(origin, direction);
                        if(temp) {
                            temp.set_element(std::distance(begin(), iter));
                            output.push_back(temp);
                        }
                    }
                    return output;
                }

                hitResult evaluateHit(const vektor & origin, const vektor & direction) const {
                    std::vector<hitResult> temp = intersect(origin, direction);
                    std::vector<double> distance;
                    for(auto iter = begin(); iter != end(); ++iter) {
                        distance.push_back(iter->get_distance(origin));
                    }
                    std::vector<double>::const_iterator minimum = std::min_element(distance.begin(), distance.end());
                    hitResult output = temp[std::distance(distance.begin(), minimum)];
                    if(std::count(distance.begin(), distance.end(), *minimum) != 1) {
                        output.set_hasHit(false);
                    }
                    return output;
                }

                std::vector<hitResult> evaluateHits(const std::vector<vektor> & origins, 
                    const std::vector<vektor> & directions) const {
                    std::vector<hitResult> output;
                    std::vector<vektor>::const_iterator origin = origins.begin();
                    std::vector<vektor>::const_iterator direction = directions.begin();
                    for( ; origin != origins.end(); ++origin, ++ direction) {
                        output.push_back(evaluateHit(*origin, *direction));
                    }
                    return output;
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
    }
}

#endif 
