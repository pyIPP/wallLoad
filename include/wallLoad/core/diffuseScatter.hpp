#ifndef include_wallLoad_core_diffuseScatter_hpp
#define include_wallLoad_core_diffuseScatter_hpp

#include <boost/python.hpp>
#include <wallLoad/core/vektor.hpp>
#include <boost/random.hpp>
#include <time.h>
#include <boost/math/constants/constants.hpp>
#include <math.h>

namespace wallLoad {
    namespace core {
        class diffuseScatter {
            public:
                diffuseScatter() :
                    m_generator(time(0)),
                    m_one(),
                    m_2pi_distribution(0, 2.0*boost::math::constants::pi<double>()) {
                    }
                virtual ~diffuseScatter() {}

                vektor get_direction(const vektor & normal) {
                    double alpha = m_2pi_distribution(m_generator);
                    double beta = asin(m_one(m_generator));
                    vektor candidate(sin(beta)*cos(alpha), sin(beta)*sin(alpha), cos(beta));

                    return candidate;
                }

            protected:
                boost::random::mt19937 m_generator;
                boost::random::uniform_01<double> m_one;
                boost::random::uniform_real_distribution<double> m_2pi_distribution;

        };
    }
}

#endif 
