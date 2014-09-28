#ifndef package_hitResult_hpp
#define package_hitResult_hpp

#include <boost/python.hpp>
#include <boost/numpy.hpp>
#include "vektor.hpp"

class hitResult
{
    public:
        hitResult() {}
        hitResult(const bool hasHit, const double distance, const vektor & hitPoint) :
            m_hasHit(hasHit), m_distance(distance), m_hitPoint(hitPoint) {
        }
        hitResult(const hitResult & rhs) : m_hasHit(rhs.m_hasHit), m_distance(rhs.m_distance), 
            m_hitPoint(rhs.m_hitPoint) {
        }
        virtual ~hitResult() {}

        hitResult & operator= (const hitResult & rhs) {
            if(this != &rhs) {
                m_hasHit = rhs.m_hasHit;
                m_distance = rhs.m_distance;
                m_hitPoint = rhs.m_hitPoint;
            }
            return *this;
        }

        bool get_hasHit() const { return m_hasHit; }
        double get_distance() const { return m_distance; }
        vektor get_hitPoint() const { return m_hitPoint; }

    protected:
        bool m_hasHit;
        double m_distance;
        vektor m_hitPoint;
};


#endif 
