#ifndef package_hitResult_hpp
#define package_hitResult_hpp

#include <boost/python.hpp>
#include <vektor.hpp>

class hitResult
{
    public:
        hitResult() {}
        hitResult(const vektor & origin, const vektor direction, const bool hasHit, const vektor & hitPoint) :
            m_origin(origin), m_direction(direction),
            m_hasHit(hasHit), m_hitPoint(hitPoint), m_element(-1) {
        }
        hitResult(const hitResult & rhs) : m_origin(rhs.m_origin), m_direction(rhs.m_direction), 
            m_hasHit(rhs.m_hasHit), m_hitPoint(rhs.m_hitPoint), m_element(rhs.m_element) {
        }
        virtual ~hitResult() {}

        hitResult & operator= (const hitResult & rhs) {
            if(this != &rhs) {
                m_origin = rhs.m_origin;
                m_direction = rhs.m_direction;
                m_hasHit = rhs.m_hasHit;
                m_hitPoint = rhs.m_hitPoint;
                m_element = rhs.m_element;
            }
            return *this;
        }

        bool get_hasHit() const { return m_hasHit; }
        void set_hasHit(const bool value) { m_hasHit = value; }
        double get_distance() const 
        { 
            return m_hasHit ? (m_origin-m_hitPoint).get_length() : 99e99;
        }
        vektor get_hitPoint() const { return m_hitPoint; }
        vektor get_origin() const { return m_origin; }
        vektor get_direction() const { return m_direction; }
        int32_t get_element() const { return m_element; }
        void set_element(const int32_t element) {
            m_element = element;
        }

    protected:
        vektor m_origin;
        vektor m_direction;
        bool m_hasHit;
        vektor m_hitPoint;
        int32_t m_element;
};

bool operator< (const hitResult & lhs, const hitResult & rhs) {
    return lhs.get_distance() < rhs.get_distance();
}

bool operator== (const hitResult & lhs, const hitResult & rhs) {
    return lhs.get_distance() == rhs.get_distance();
}


#endif 
