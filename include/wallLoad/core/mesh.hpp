#ifndef include_wallLoad_core_mesh_hpp
#define include_wallLoad_core_mesh_hpp

#include <boost/python.hpp>
#include <boost/random.hpp>
#include <time.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <fstream>
#include <wallLoad/core/vertex.hpp>
#include <wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        class mesh : public std::vector<vertex>
        {
            public:
                mesh(const mesh & rhs) : 
                    std::vector<vertex>(rhs),
                    m_emissivity(rhs.size(), 1.0),
                    m_generator(time(0)),
                    m_uniform() {
                }

                mesh(const boost::python::list & rhs) : 
                    std::vector<vertex>(),
                    m_emissivity(boost::python::len(rhs), 1.0),
                    m_generator(time(0)),
                    m_uniform() {
                    for(uint32_t i = 0; i < boost::python::len(rhs); ++i){
                        std::vector<vertex>::push_back(boost::python::extract<vertex>(rhs[i]));
                    }
                }

                mesh(const std::string & filename) : 
                    std::vector<vertex>(),
                    m_emissivity(),
                    m_generator(time(0)),
                    m_uniform() {
                    std::fstream file(filename.c_str(), std::ios::in);
                    if(file.is_open()) {
                        std::string temp;
                        while(temp!="$Nodes") {
                            std::getline(file, temp, '\n');
                        }
                        uint32_t nNodes;
                        file >> nNodes;
                        std::vector<vektor> nodes;
                        uint32_t nodeNumber;
                        double x,y,z;
                        for(uint32_t i = 0; i < nNodes; ++i) {
                            file >> nodeNumber >> x >> y >> z;
                            nodes.push_back(vektor(x,y,z));
                        }
                        while(temp!="$Elements") {
                            std::getline(file, temp, '\n');
                        }
                        uint32_t nElements;
                        file >> nElements;
                        uint32_t elementId, elementType;
                        uint32_t nTags, tag;
                        uint32_t node1, node2, node3;
                        for(uint32_t i = 0; i < nElements; ++i) {
                            file >> elementId >> elementType;
                            if(elementType==2) {
                                file >> nTags;
                                for(uint32_t j = 0; j < nTags; ++j) {
                                    file >> tag;
                                }
                                file >> node1 >> node2 >> node3;
                                std::vector<vertex>::push_back(vertex(nodes[node1-1], nodes[node2-1], nodes[node3-1]));
                                m_emissivity.push_back(1.0);
                            }
                            else {
                                std::getline(file, temp, '\n');
                            }
                        }

                        file.close();
                    }
                }

                virtual ~mesh() {}

                mesh & operator=(const mesh & rhs) {
                    if(this != &rhs) {
                        std::vector<vertex>::operator=(rhs);
                        m_emissivity = rhs.m_emissivity;
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
                            temp.element = std::distance(begin(), iter);
                            output.push_back(temp);
                        }
                    }
                    return output;
                }

                hitResult evaluateHit(const vektor & origin, const vektor & direction) const {
                    std::vector<hitResult> temp = intersect(origin, direction);
                    if(temp.size()==0) {
                        return hitResult();
                    }
                    std::vector<double> distance;
                    for(auto iter = temp.begin(); iter != temp.end(); ++iter) {
                        distance.push_back(iter->get_distance(origin));
                    }
                    std::vector<double>::iterator minimum = std::min_element(distance.begin(), distance.end());
                    uint32_t element = std::distance(distance.begin(), minimum);
                    hitResult output = temp[std::distance(distance.begin(), minimum)];
                    if(std::count(distance.begin(), distance.end(), *minimum) != 1) {
                        output.hasHit = false;
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

                boost::python::list evaluateHits_python(const boost::python::list & origins, 
                    const boost::python::list & directions) {
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
                        if(temp.hasHit) {
                            output.append(temp);
                        }
                    }
                    return output;
                }
                vertex & operator[] (const uint32_t i) { 
                    return std::vector<vertex>::operator[](i);
                }

                std::vector<double> get_areas() const {
                    std::vector<double> output;
                    for(std::vector<vertex>::const_iterator iter = begin(); iter != end(); ++iter) {
                        output.push_back(iter->get_area());
                    }
                    return output;
                }
            protected:
                std::vector<double> m_emissivity;       
                boost::random::mt19937 m_generator;
                boost::random::uniform_01<double> m_uniform;

        };
    }
}

#endif 
