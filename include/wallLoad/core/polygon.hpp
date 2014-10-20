#ifndef include_wallLoad_core_polygon_hpp
#define include_wallLoad_core_polygon_hpp

#include <boost/python.hpp>
#include <vector>
#include <wallLoad/core/vektor.hpp>
#include <stdint.h>

namespace wallLoad {
    namespace core {
        class polygon {
            public:
                polygon() : m_R(), m_z() {}
                polygon(const std::vector<double> & R, const std::vector<double> & z) 
                    : m_R(R), m_z(z) {
                }
                polygon(const polygon & rhs) 
                    : m_R(rhs.m_R), m_z(rhs.m_z) {
                }
                virtual ~polygon() {
                }
                polygon(const boost::python::list & R, const boost::python::list & z)
                    : m_R(), m_z() {
                    for(uint32_t i = 0; i < boost::python::len(R); ++i){
                        m_R.push_back(boost::python::extract<double>(R[i]));
                        m_z.push_back(boost::python::extract<double>(z[i]));
                    }
                }

                polygon & operator= (const polygon & rhs) {
                    if(this != &rhs) {
                        m_R = rhs.m_R;
                        m_z = rhs.m_z;
                    }
                    return *this;
                }

                std::vector<double> get_R() const {
                    return std::vector<double>(m_R.begin(), m_R.end());
                }

                std::vector<double> get_z() const {
                    return std::vector<double>(m_z.begin(), m_z.end());
                }

                boost::python::list get_R_python() const {
                    boost::python::list list;
                    for(std::vector<double>::const_iterator iter = m_R.begin(); iter != m_R.end(); ++iter) {
                        list.append(*iter);
                    }
                    return list;
                }

                boost::python::list get_z_python() const {
                    boost::python::list list;
                    for(std::vector<double>::const_iterator iter = m_z.begin(); iter != m_z.end(); ++iter) {
                        list.append(*iter);
                    }
                    return list;
                }

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

                uint32_t size() const {
                    return m_R.size();
                }

            protected:
                std::vector<double> m_R;
                std::vector<double> m_z;



                
        };
    }
}

#endif 
