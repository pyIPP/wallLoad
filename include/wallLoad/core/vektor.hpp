#ifndef include_wallLoad_core_vektor_hpp
#define include_wallLoad_core_vektor_hpp

#include <boost/python.hpp>
#include <math.h>
#include <iostream>
#include <vector>

namespace wallLoad {
    namespace core {
        class vektor
        {
            public:
                vektor() : m_x(0), m_y(0), m_z(0) {}
                vektor(const double x, const double y, const double z) : m_x(x), m_y(y), m_z(z) {}
                vektor(const vektor & rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_z(rhs.m_z) {}
                vektor(const boost::python::list & list) {
                    m_x = boost::python::extract<double>(list[0]);
                    m_y = boost::python::extract<double>(list[1]);
                    m_z = boost::python::extract<double>(list[2]);
                }
                vektor(const boost::python::dict & dict) {
                    m_x = boost::python::extract<double>(dict.get("x"));
                    m_y = boost::python::extract<double>(dict.get("y"));
                    m_z = boost::python::extract<double>(dict.get("z"));
                }
                vektor(const std::vector<double> & rhs) : m_x(rhs[0]), m_y(rhs[1]), m_z(rhs[2]) {
                }
                vektor(const vektor & left, const vektor & right) :
                    m_x(left.m_x - right.m_x), 
                    m_y(left.m_y - right.m_y),
                    m_z(left.m_z - right.m_z) {
                }

                virtual ~vektor() {}

                vektor & operator= (const vektor & rhs) {
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

                inline double get_length() const {
                    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
                }

                inline double get_dot_product(const vektor & rhs) const {
                    return m_x*rhs.m_x + m_y*rhs.m_y + m_z*rhs.m_z;
                }

                inline vektor get_cross_product(const vektor & rhs) const {
                    return vektor(m_y*rhs.m_z-m_z*rhs.m_y, m_z*rhs.m_x-m_x*rhs.m_z, m_x*rhs.m_y-m_y*rhs.m_x);
                }

                inline vektor & operator+=(const vektor & rhs) {
                    m_x += rhs.m_x;
                    m_y += rhs.m_y;
                    m_z += rhs.m_z;
                    return *this;
                }

                inline vektor & operator-=(const vektor & rhs) {
                    m_x -= rhs.m_x;
                    m_y -= rhs.m_y;
                    m_z -= rhs.m_z;
                    return *this;
                }

                inline vektor & operator*= (const double rhs) {
                    m_x *= rhs;
                    m_y *= rhs;
                    m_z *= rhs;
                    return *this;
                }

                inline vektor & operator/= (const double rhs) {
                    m_x /= rhs;
                    m_y /= rhs;
                    m_z /= rhs;
                    return *this;
                }

                inline void normalize() {
                    *this /= get_length();
                }

                inline vektor get_normalized() const {
                    double length = get_length();
                    return vektor(m_x/length, m_y/length, m_z/length);
                }

                inline vektor get_normal_vektor() const {
                    vektor output(m_y, -m_x, 0);
                    output /= output.get_length();
                    return output;
                }

                inline vektor operator -() const {
                    return vektor(-m_x, -m_y, -m_z);
                }

                inline double get_angle(const vektor & rhs) const {
                    return acos(get_dot_product(rhs)/get_length()/rhs.get_length());
                }

        //        boost::numpy::ndarray asarray() const {
        //            boost::python::tuple tuple = boost::python::make_tuple(3);
        //            boost::numpy::ndarray output = boost::numpy::empty(tuple, boost::numpy::dtype::get_builtin<double>());
        //            *((double *)output.get_data()) = m_x;
        //            *((double *)output.get_data() + 1) = m_y;
        //            *((double *)output.get_data() + 2) = m_z;
        //            return output;
        //        }

                boost::python::list to_list() const {
                    boost::python::list output;
                    output.append(m_x);
                    output.append(m_y);
                    output.append(m_z);
                    return output;
                }

                boost::python::dict to_dict() const {
                    boost::python::dict dict;
                    dict.setdefault("x", m_x);
                    dict.setdefault("y", m_y);
                    dict.setdefault("z", m_z);
                    return dict;
                }

                inline vektor get_rotated_x(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(m_x, m_y*cosa - m_z*sina, m_y*sina + m_z*cosa);
                }

                inline vektor get_rotated_y(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(m_x*cosa + m_z*sina, m_y, m_z*cosa - m_x*sina);
                }

                inline vektor get_rotated_z(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(m_x*cosa - m_y*sina, m_x*sina + m_y*cosa, m_z);
                }

                inline double get_distance(const vektor & rhs) const {
                    return sqrt(pow(m_x-rhs.m_x,2.0) + pow(m_y-rhs.m_y,2.0) + pow(m_z-rhs.m_z,2.0));
                }

            protected:
                double m_x;
                double m_y;
                double m_z;
        };

        std::ostream & operator<< (std::ostream & ostr, const vektor & vek) {
            ostr << vek.get_x() << '\t' << vek.get_y() << '\t' << vek.get_z();
            return ostr;
        }

        inline vektor operator+ (const vektor & lhs, const vektor & rhs) {
            return vektor(lhs) += rhs;
        }

        inline vektor operator- (const vektor & lhs, const vektor & rhs) {
            return vektor(lhs) -= rhs;
        }

        inline vektor operator* (const vektor & lhs, const double value) {
            return vektor(lhs) *= value;
        }

        inline vektor operator* (const double value, const vektor & rhs) {
            return vektor(rhs) *= value;
        }

        inline vektor operator/ (const vektor & lhs, const double value) {
            return vektor(lhs) /= value;
        }

        inline bool operator== (const vektor & lhs, const vektor & rhs) {
            return lhs.get_x()==rhs.get_x() && lhs.get_y()==rhs.get_y() && lhs.get_z()==rhs.get_z();
        }

        inline bool operator!= (const vektor & lhs, const vektor & rhs) {
            return !(lhs == rhs);
        }

    }
}

#endif
