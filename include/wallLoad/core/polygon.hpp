#ifndef include_wallLoad_core_polygon_hpp
#define include_wallLoad_core_polygon_hpp

#include <boost/python.hpp>
#include <vector>
#include <wallLoad/core/vektor.hpp>
#include <stdint.h>

namespace wallLoad {
    namespace core {
        /*! \brief Class representing a polygon
         *
         * This class represents a polygon in \f$(R,z)\f$ coordinates.
         */
        class polygon {
            public:
                /*! \brief Default constructor
                 *
                 * This constructor initializes an empty polygon.
                 */
                polygon() : m_R(), m_z() {}
                /*! \brief Constructor
                 *
                 * This constructor initializes the polygon with the given \f$(R,z)\f$ points.
                 */
                polygon(const std::vector<double> & R, const std::vector<double> & z) 
                    : m_R(R), m_z(z) {
                }
                /*! \brief Copy constructor */
                polygon(const polygon & rhs) 
                    : m_R(rhs.m_R), m_z(rhs.m_z) {
                }
                /*! \brief Destructor */
                virtual ~polygon() {
                }
                /*! \brief Python constructor
                 *
                 * This constructor initializes the polygon with the given \f$(R,z)\f$ points.
                 * This constructor is intended as interface to python.
                 * Do not use this constructor from within C++.
                 */
                polygon(const boost::python::list & R, const boost::python::list & z)
                    : m_R(), m_z() {
                    for(uint32_t i = 0; i < boost::python::len(R); ++i){
                        m_R.push_back(boost::python::extract<double>(R[i]));
                        m_z.push_back(boost::python::extract<double>(z[i]));
                    }
                }

                /*! \brief Assignment operator
                 *
                 * This operator copies the points of the given polygon to the current instance.
                 */
                polygon & operator= (const polygon & rhs) {
                    if(this != &rhs) {
                        m_R = rhs.m_R;
                        m_z = rhs.m_z;
                    }
                    return *this;
                }

                /*! \brief Get \f$R\f$ coordinates.
                 *
                 * This function returns the \f$R\f$ coordinates of the polygon.
                 */
                std::vector<double> get_R() const {
                    return std::vector<double>(m_R.cbegin(), m_R.cend());
                }

                /*! \brief Get \f$z\f$ coordinates.
                 *
                 * This function returns the \f$z\f$ coordinates of the polygon.
                 */
                std::vector<double> get_z() const {
                    return std::vector<double>(m_z.cbegin(), m_z.cend());
                }

                /*! \brief Get \f$R\f$ coordinates as python list.
                 *
                 * This function returns the \f$R\f$ coordinates of the polygon.
                 * This functions is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_R_python() const {
                    boost::python::list list;
                    for(std::vector<double>::const_iterator iter = m_R.begin(); iter != m_R.end(); ++iter) {
                        list.append(*iter);
                    }
                    return list;
                }

                /*! \brief Get \f$z\f$ coordinates as python list.
                 *
                 * This function returns the \f$z\f$ coordinates of the polygon.
                 * This functions is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_z_python() const {
                    boost::python::list list;
                    for(std::vector<double>::const_iterator iter = m_z.begin(); iter != m_z.end(); ++iter) {
                        list.append(*iter);
                    }
                    return list;
                }

                /*! \brief Check if point \f$(x,y)\f$ is inside the polygon 
                 *
                 * This function checks if the point \f$(x,y)\f$ lies within the polygon.
                 */
                bool inside(const double x, const double y) const {
                    bool inside = false;
                    uint32_t N = m_R.size();
                    double x1 = m_R[N-1];
                    double y1 = m_z[N-1];
                    double x2 = m_R[0];
                    double y2 = m_z[0];
                    bool startUeber = y1 >= y? true : false;
                    bool endUeber;
                    for(uint32_t i = 1; i < N ; ++i) {
                        endUeber = y2 >= y? true : false;
                        if(startUeber != endUeber) {
                            if((y2 - y)*(x2 - x1) <= (y2 - y1)*(x2 - x)) {
                                if(endUeber) {
                                    inside = !inside;
                                }
                            }
                            else {
                                if(!endUeber) {
                                    inside = !inside;
                                }
                            }
                        }
                        startUeber = endUeber;
                        y1 = y2;
                        x1 = x2;
                        x2 = m_R[i];
                        y2 = m_z[i];
                    }
                    return inside;
                }

                /*! \brief Number of points in the polygon */
                uint32_t size() const {
                    return m_R.size();
                }

            protected:
                std::vector<double> m_R; /*!< \brief \f$R\f$ coordinates of the polygon */
                std::vector<double> m_z; /*!< \brief \f$z\f$ coordinates of the polygon */



                
        };
    }
}

#endif 
