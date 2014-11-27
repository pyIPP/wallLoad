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
        /*! \brief Class representing the mesh of the first wall.
         *
         * This class stores the vertices of the first wall contour.
         */
        class mesh : public std::vector<vertex>
        {
            public:
                /*! \brief Copy constructor */
                mesh(const mesh & rhs) : 
                    std::vector<vertex>(rhs),
                    m_emissivity(rhs.size(), 1.0),
                    m_generator(time(0)),
                    m_uniform() {
                }

                /*! \brief Python constructor
                 *
                 * This constructor loads the vertices from a python list.
                 * This constructor is intended as python interface.
                 * Do not use this constructor from within C++.
                 */
                mesh(const boost::python::list & rhs) : 
                    std::vector<vertex>(),
                    m_emissivity(boost::python::len(rhs), 1.0),
                    m_generator(time(0)),
                    m_uniform() {
                    for(uint32_t i = 0; i < boost::python::len(rhs); ++i){
                        std::vector<vertex>::push_back(boost::python::extract<vertex>(rhs[i]));
                    }
                }

                /*! Constructor
                 *
                 * This constructor loads the vertices from a *.msh file created by gmsh.
                 */
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

                /*! \brief Destructor */
                virtual ~mesh() {}

                /*! \brief Assignment operator
                 *
                 * This operators copies the vertices from the given mesh to the current instance.
                 */
                mesh & operator=(const mesh & rhs) {
                    if(this != &rhs) {
                        std::vector<vertex>::operator=(rhs);
                        m_emissivity = rhs.m_emissivity;
                    }
                    return *this;
                }

                /*! \brief Append vertex
                 *
                 * This function appends the given vertex to the mesh.
                 */
                inline void append(const vertex & rhs) {
                    std::vector<vertex>::push_back(rhs);
                }

                /*! \brief Calculate the intersections of the given ray with the mesh.
                 *
                 * This function calculates the intersections of the given ray with the mesh.
                 * All points that hit a vertex are returned.
                 * The sorting which point is the real hit point needs to be done separately.
                 */
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

                /*! \brief Calculate the hit point of the ray
                 *
                 * This function calculates the intersections of the given ray with the mesh.
                 * Only the real hit point of the ray is returned.
                 * This means only points that lie in the direction of the ray and from those the one closest to the origin.
                 */
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

                /*! \brief Calculate the hit points of the rays
                 *
                 * This function calculates the intersections of each ray with the mesh.
                 * Only the real hit point of the each ray is returned.
                 * This means only points that lie in the direction of the ray and from those the one closest to the origin.
                 */
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

                /*! \brief Calculate the hit points of the rays and return them as python list.
                 *
                 * This function calculates the intersections of each ray with the mesh.
                 * Only the real hit point of the each ray is returned.
                 * This means only points that lie in the direction of the ray and from those the one closest to the origin.
                 * This function is intended as python interface.
                 * Do not use it from within C++.
                 */
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

                /*! \brief Get ith vertex of the mesh. */
                vertex & operator[] (const uint32_t i) { 
                    return std::vector<vertex>::operator[](i);
                }

                /*! \brief Calculate the areas of the vertices.
                 *
                 * This function calculates the area of each vertex in the mesh and returns the result as an array.
                 */
                std::vector<double> get_areas() const {
                    std::vector<double> output;
                    for(std::vector<vertex>::const_iterator iter = begin(); iter != end(); ++iter) {
                        output.push_back(iter->get_area());
                    }
                    return output;
                }
            protected:
                std::vector<double> m_emissivity; /*!< \brief Emissivity of the wall elements. */
                boost::random::mt19937 m_generator; /*!< \brief Random number generator */
                boost::random::uniform_01<double> m_uniform; /*!< \brief Uniform random distribution \f$[0,1[\f$. */

        };
    }
}

#endif 
