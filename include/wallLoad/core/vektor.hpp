#ifndef include_wallLoad_core_vektor_hpp
#define include_wallLoad_core_vektor_hpp

#include <boost/python.hpp>
#include <math.h>
#include <iostream>
#include <vector>

namespace wallLoad {
    namespace core {
        /*! \brief Class representing a vector in 3D.
         *
         * This class represents a vector \f$(x,y,z)\f$.
         * The basic functions for vector calculus are implemented.
         */
        class vektor
        {
            public:
                /*! \brief Default constructor
                 *
                 * This constructor initializes the vector with zero.
                 */
                vektor() : x(0.0), y(0.0), z(0.0) {}
                /*! \brief Constructor
                 *
                 * This constructor initialized the vector with \f$(x,y,z)\f$.
                 */
                vektor(const double yIn, const double xIn, const double zIn) : x(xIn), y(yIn), z(zIn) {}
                /*! \brief Copy constructor */
                vektor(const vektor & rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
                /*! \brief Python constructor from list
                 *
                 * This constructor initializes the vector from a python list.
                 * This constructor is only intended to interface with python.
                 * Do not use this constructor from within C++.
                 */
                vektor(const boost::python::list & list) {
                    x = boost::python::extract<double>(list[0]);
                    y = boost::python::extract<double>(list[1]);
                    z = boost::python::extract<double>(list[2]);
                }
                /*! \brief Python constructor from dictionary
                 *
                 * This constructor initializes the vector from a python dictionary.
                 * This constructor is only intended to interface with python.
                 * Do not use this constructor from within C++.
                 */
                vektor(const boost::python::dict & dict) {
                    x = boost::python::extract<double>(dict.get("x"));
                    y = boost::python::extract<double>(dict.get("y"));
                    z = boost::python::extract<double>(dict.get("z"));
                }
                /*! \brief Constructor
                 *
                 * This constructor initializes the vector from a std::vector instance.
                 */
                vektor(const std::vector<double> & rhs) : x(rhs[0]), y(rhs[1]), z(rhs[2]) {
                }
                /*! \brief Difference constructor
                 *
                 * This constructor initializes the vector as the difference of the given vectors.
                 * \f$\mathbf{v_0} - \mathbf{v_1}\f$.
                 */
                vektor(const vektor & left, const vektor & right) :
                    x(left.x - right.x), 
                    y(left.y - right.y),
                    z(left.z - right.z) {
                }
                /*! Destructor */
                virtual ~vektor() {}

                vektor & operator= (const vektor & rhs) {
                    if(this != &rhs) {
                        x = rhs.x;
                        y = rhs.y;
                        z = rhs.z;
                    }
                    return *this;
                }

                inline double get_length() const {
                    return sqrt(x*x + y*y + z*z);
                }

                inline double get_dot_product(const vektor & rhs) const {
                    return x*rhs.x + y*rhs.y + z*rhs.z;
                }

                inline vektor get_cross_product(const vektor & rhs) const {
                    return vektor(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
                }

                inline vektor & operator+=(const vektor & rhs) {
                    x += rhs.x;
                    y += rhs.y;
                    z += rhs.z;
                    return *this;
                }

                inline vektor & operator-=(const vektor & rhs) {
                    x -= rhs.x;
                    y -= rhs.y;
                    z -= rhs.z;
                    return *this;
                }

                inline vektor & operator*= (const double rhs) {
                    x *= rhs;
                    y *= rhs;
                    z *= rhs;
                    return *this;
                }

                inline vektor & operator/= (const double rhs) {
                    x /= rhs;
                    y /= rhs;
                    z /= rhs;
                    return *this;
                }

                inline void normalize() {
                    *this /= get_length();
                }

                inline vektor get_normalized() const {
                    double length = get_length();
                    return vektor(x/length, y/length, z/length);
                }

                inline vektor get_normal_vektor() const {
                    vektor output(y, -x, 0);
                    output /= output.get_length();
                    return output;
                }

                inline vektor operator -() const {
                    return vektor(-x, -y, -z);
                }

                inline double get_angle(const vektor & rhs) const {
                    return acos(get_dot_product(rhs)/get_length()/rhs.get_length());
                }

                boost::python::list to_list() const {
                    boost::python::list output;
                    output.append(x);
                    output.append(y);
                    output.append(z);
                    return output;
                }

                boost::python::dict to_dict() const {
                    boost::python::dict dict;
                    dict.setdefault("x", x);
                    dict.setdefault("y", y);
                    dict.setdefault("z", z);
                    return dict;
                }

                inline vektor get_rotated_x(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(x, y*cosa - z*sina, y*sina + z*cosa);
                }

                inline vektor get_rotated_y(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(x*cosa + z*sina, y, z*cosa - x*sina);
                }

                inline vektor get_rotated_z(const double alpha) const {
                    double cosa = cos(alpha);
                    double sina = sin(alpha);
                    return vektor(x*cosa - y*sina, x*sina + y*cosa, z);
                }

                /*! \brief Get the distance
                 *
                 * This function calculates the distance between the two vectors.
                 * \f$ d = \sqrt{(x_1-x_0)^2 + (y_1-y_0)^2 + (z_1 - z_1)^2 }\f$
                 */ 
                inline double get_distance(const vektor & rhs) const {
                    return sqrt(pow(x-rhs.x,2.0) + pow(y-rhs.y,2.0) + pow(z-rhs.z,2.0));
                }

            public:
                double x; /*< \brief x component of the vector */
                double y; /*< \brief y component of the vector */
                double z; /*< \brief z component of the vector */
        };

        std::ostream & operator<< (std::ostream & ostr, const vektor & vek) {
            ostr << vek.x << '\t' << vek.y << '\t' << vek.z;
            return ostr;
        }

        inline vektor operator+ (const vektor & lhs, const vektor & rhs) {
            return vektor(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
        }

        inline vektor operator- (const vektor & lhs, const vektor & rhs) {
            return vektor(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
        }

        inline vektor operator* (const vektor & lhs, const double value) {
            return vektor(lhs.x*value, lhs.y*value, lhs.z*value);
        }

        inline vektor operator* (const double value, const vektor & rhs) {
            return vektor(rhs.x*value, rhs.y*value, rhs.z*value);
        }

        inline vektor operator/ (const vektor & lhs, const double value) {
            return vektor(lhs.x/value, lhs.y/value, lhs.z/value);
        }

        inline bool operator== (const vektor & lhs, const vektor & rhs) {
            return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
        }

        inline bool operator!= (const vektor & lhs, const vektor & rhs) {
            return !(lhs == rhs);
        }

    }
}

#endif
