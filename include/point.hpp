#ifndef package_point_hpp
#define package_point_hpp

#include <boost/python.hpp>
//#include <boost/numpy.hpp>
#include <iostream>
#include <math.h>

class point
{
    public:
        point() : m_x(0), m_y(0), m_z(0) {}
        point(const double x, const double y, const double z) : m_x(x), m_y(y), m_z(z) {}
        point(const point & rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_z(rhs.m_z) {}
        point(const boost::python::list & list) : 
            m_x(boost::python::extract<double>(list[0])),
            m_y(boost::python::extract<double>(list[1])),
            m_z(boost::python::extract<double>(list[2])) {
        }
//        point(const boost::numpy::ndarray & data) {
//            boost::numpy::ndarray temp = data.astype(boost::numpy::dtype::get_builtin<double>());
//            m_x = *((double *)temp.get_data());
//            m_y = *((double *)temp.get_data() + 1);
//            m_z = *((double *)temp.get_data() + 2);
//        }

        virtual ~point() {}

        point & operator= (const point & rhs) {
            if(this != &rhs) {
                m_x = rhs.m_x;
                m_y = rhs.m_y;
                m_z = rhs.m_z;
            }
            return *this;
        }

        inline double get_x() const { return m_x; }
        inline double get_y() const { return m_y; }
        inline double get_z() const { return m_z; }

        inline void set_x(const double x) { m_x = x; }
        inline void set_y(const double y) { m_y = y; }
        inline void set_z(const double z) { m_z = z; }

        point rotate_toroidal(const double alpha) const {
            double cosa = cos(alpha);
            double sina = sin(alpha);
            return point(m_x*cosa - m_y*sina, m_x*sina + m_y*cosa, m_z);
        }

        
    protected:
        double m_x;
        double m_y;
        double m_z;  
};



#endif 
