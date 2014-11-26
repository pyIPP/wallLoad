#ifndef include_wallLoad_core_vertex_hpp
#define include_wallLoad_core_vertex_hpp

#define EPSILON 0.000001

#include <boost/python.hpp>
#include <iostream>
#include <wallLoad/core/vektor.hpp>
#include "hitResult.hpp"
#include <cmath>
namespace wallLoad {
    namespace core {
        /*! \brief Class representing a vertex with three points
         *
         * This class represents a vertex with three points in 3D.
         * The vertex can be checked for the intersection of a vector origination from a arbitrary point in space.
         */
        class vertex
        {
        public:
            /*! \brief Constructor
             *
             * This constructor initializes the vertex with the given points.
             */
            vertex(const vektor & P1, const vektor & P2, const vektor & P3) :
                p1(P1), p2(P2), p3(P3) {
            }
            /*! \brief Copy constructor*/
            vertex(const vertex & vert) : p1(vert.p1), p2(vert.p2), p3(vert.p3) {
            }
            /*! \brief Constructor
             *
             * This constructor initializes the vertex with the first three points in the given vector.
             */
            vertex(const std::vector<vektor> & rhs) : p1(rhs[0]), p2(rhs[1]), p3(rhs[2]) {
            }
            /*! \brief Python constructor
             *
             * This constructor initializes the vertex with the points given as three lists.
             * This constructor is intended as interface to python.
             * Do not use this constructor from within C++.
             */
            vertex(const boost::python::list & P1, const boost::python::list & P2, const boost::python::list & P3) :
                p1(P1), p2(P2), p3(P3) {
            }
            /*! \brief Python constructor
             *
             * This constructor initializes the vertex with the points given as a list of lists.
             * This constructor is intended as interface to python.
             * Do not use this constructor from within C++.
             */
            vertex(const boost::python::list & list) :
                p1(boost::python::extract<boost::python::list>(list[0])),
                p2(boost::python::extract<boost::python::list>(list[1])),
                p3(boost::python::extract<boost::python::list>(list[2])) {
            }
            /*! \brief Destructor. */
            virtual ~vertex() {
            }

            /*! \brief Assignment operator.
             *
             * This operator copies the points from the given vertex to the current instance.
             */
            vertex & operator= (const vertex & rhs) {
                if(this != &rhs) {
                    p1 = rhs.p1;
                    p2 = rhs.p2;
                    p3 = rhs.p3;
                }
                return *this;
            }

            /*! \brief Get intersection
             *
             * This function calculates if the ray origination from the given position intersects the vertex.
             * If the vertex is intersected the position of the intersection is returned.
             * The algorithm used for the calculation is the Möller-Trumbore algorithm.
             * \param origin Position from where the ray originates.
             * \param direction The direction in which the ray travels.
             */
            hitResult intersect(const vektor & origin, const vektor & direction) const {
                vektor e1, e2;
                vektor P, Q, T;
                double det, inv_det, u, v;
                double t;
                e1 = p2 - p1;
                e2 = p3 - p1;
                P = direction.get_cross_product(e2);
                det = e1.get_dot_product(P);
                if( det > -EPSILON && det < EPSILON) {
                    return hitResult(false, vektor());
                }
                inv_det = 1.0/det;
                T = origin - p1;
                u = T.get_dot_product(P) * inv_det;
                if(u < 0.0 || u > 1.0) {
                    return hitResult(false, vektor());
                }
                Q = T.get_cross_product(e1);
                v = direction.get_dot_product(Q)*inv_det;
                if(v < 0.0 || u + v  > 1.0) {
                    return hitResult(false, vektor());
                }
                t = e2.get_dot_product(Q) * inv_det;
                if(t > EPSILON) {
                    return hitResult(true, origin + t*direction);
                }
                return hitResult(false, origin + t*direction);
            }

            /*! \brief Calculate the area of the vertex
             *
             * This function calculates the area of the vertex.
             */
            double get_area() const {
               double a = (p1 - p2).get_length();
               double b = (p2 - p3).get_length();
               double c = (p3 - p1).get_length();
               double s = a/2.0 + b/2.0 + c/2.0;
               return std::sqrt(s*(s-a)*(s-b)*(s-c));
            }

            /*! \brief Calculate the normal vector of the vertex.
             *
             * This function returns the normal vector on the vertex.
             */
            vektor get_normal() {
                return ((p2 - p1).get_cross_product(p3 - p1)).get_normalized();
            }

            /*! \brief Calculate the center of the vertex
             *
             * This function returns the center of the vertex
             * \f$ \mathrm{c} \ \frac{\mathbf{p}_1 + \mathbf{p}_2 + \mathbf{p}_3}{3}\f$
             */
            vektor get_center() const {
                return (p1 + p2 + p3)/3.0;
            }

        public:
            vektor p1; /*!< \brief First point of vertex */
            vektor p2; /*!< \brief Second point of vertex */
            vektor p3; /*!< \brief Thrid point of vertex */
        };
    }
}

#endif 
