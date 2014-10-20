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
        class vertex
        {
        public:
            vertex(const vektor & p1, const vektor & p2, const vektor & p3) :
                m_p1(p1), m_p2(p2), m_p3(p3) {
            }
            vertex(const vertex & vert) : m_p1(vert.m_p1), m_p2(vert.m_p2), m_p3(vert.m_p3) {
            }
            vertex(const std::vector<vektor> & rhs) : m_p1(rhs[0]), m_p2(rhs[1]), m_p3(rhs[2]) {
            }
            vertex(const boost::python::list & p1, const boost::python::list & p2, const boost::python::list & p3) :
                m_p1(p1), m_p2(p2), m_p3(p3) {
            }
            vertex(const boost::python::list & list) :
                m_p1(boost::python::extract<boost::python::list>(list[0])),
                m_p2(boost::python::extract<boost::python::list>(list[1])),
                m_p3(boost::python::extract<boost::python::list>(list[2])) {
            }
            virtual ~vertex() {
            }

            vertex & operator= (const vertex & rhs) {
                if(this != &rhs) {
                    m_p1 = rhs.m_p1;
                    m_p2 = rhs.m_p2;
                    m_p3 = rhs.m_p3;
                }
                return *this;
            }

            vektor get_p1() const { return m_p1; }
            vektor get_p2() const { return m_p2; }
            vektor get_p3() const { return m_p3; }

            void set_p1(const vektor & p1) { m_p1 = p1; }
            void set_p2(const vektor & p2) { m_p2 = p2; }
            void set_p3(const vektor & p3) { m_p3 = p3; }

            hitResult intersect(const vektor & origin, const vektor & direction) const {
                // Möller–Trumbore intersection algorithm
                vektor e1, e2;
                vektor P, Q, T;
                double det, inv_det, u, v;
                double t;
                e1 = m_p2 - m_p1;
                e2 = m_p3 - m_p1;
                P = direction.get_cross_product(e2);
                det = e1.get_dot_product(P);
                if( det > -EPSILON && det < EPSILON) {
                    return hitResult(false, vektor());
                }
                inv_det = 1.0/det;
                T = origin - m_p1;
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

            double get_area() const {
               double a = (m_p1 - m_p2).get_length();
               double b = (m_p2 - m_p3).get_length();
               double c = (m_p3 - m_p1).get_length();
               double s = a/2.0 + b/2.0 + c/2.0;
               return std::sqrt(s*(s-a)*(s-b)*(s-c));
            }

            vektor get_normal() {
                return ((m_p2 - m_p1).get_cross_product(m_p3 - m_p1)).get_normalized();
            }

        protected:
            vektor m_p1;
            vektor m_p2;
            vektor m_p3;  
        };
    }
}

#endif 
