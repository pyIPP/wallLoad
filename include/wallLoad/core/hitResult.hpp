#ifndef include_wallLoad_core_hitResult_hpp
#define include_wallLoad_core_hitResult_hpp

#include <boost/python.hpp>
#include <wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        class hitResult
        {
            public:
                hitResult(const bool hasHit, const vektor & hitPoint) :
                    m_hasHit(hasHit), m_hitPoint(hitPoint) {
                }
                hitResult(const hitResult & rhs) :
                    m_hasHit(rhs.m_hasHit), m_hitPoint(rhs.m_hitPoint) {
                }
                virtual ~hitResult() {}

                hitResult & operator= (const hitResult & rhs) {
                    if(this != &rhs) {
                        m_hasHit = rhs.m_hasHit;
                        m_hitPoint = rhs.m_hitPoint;
                    }
                    return *this;
                }
                bool get_hasHit() const { return m_hasHit; }
                void set_hasHit(const bool value) { return m_hasHit; }
                vektor get_hitPoint() const { return m_hitPoint; }
                explicit bool() const { return m_hasHit; }
                double get_distance(const vektor & rhs) const {
                    return (m_hitPoint - rhs).get_length();
                }

            protected:
                bool m_hasHit;
                vektor m_hitPoint;
        };
    }
}


#endif 
