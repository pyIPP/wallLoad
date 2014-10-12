#ifndef include_wallLoad_core_hitResult_hpp
#define include_wallLoad_core_hitResult_hpp

#include <boost/python.hpp>
#include <wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        class hitResult
        {
            public:
                hitResult() : m_hasHit(false), m_hitPoint(), m_element(-1) {}
                hitResult(const bool hasHit, const vektor & hitPoint) :
                    m_hasHit(hasHit), m_hitPoint(hitPoint), m_element(-1) {
                }
                hitResult(const hitResult & rhs) :
                    m_hasHit(rhs.m_hasHit), m_hitPoint(rhs.m_hitPoint),
                    m_element(rhs.m_element) {
                }
                virtual ~hitResult() {}

                hitResult & operator= (const hitResult & rhs) {
                    if(this != &rhs) {
                        m_hasHit = rhs.m_hasHit;
                        m_hitPoint = rhs.m_hitPoint;
                        m_element = rhs.m_element;
                    }
                    return *this;
                }
                bool get_hasHit() const { return m_hasHit; }
                void set_hasHit(const bool value) { m_hasHit = value; }
                vektor get_hitPoint() const { return m_hitPoint; }
                explicit operator bool() const { return m_hasHit; }
                double get_distance(const vektor & rhs) const {
                    return (m_hitPoint - rhs).get_length();
                }

                int32_t get_element() const { return m_element; }
                void set_element(const int32_t element) { m_element = element; }

            protected:
                bool m_hasHit;
                vektor m_hitPoint;
                int32_t m_element;
        };
    }
}


#endif 
