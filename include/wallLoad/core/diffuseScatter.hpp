#ifndef include_wallLoad_core_diffuseScatter_hpp
#define include_wallLoad_core_diffuseScatter_hpp

#include <boost/python.hpp>
#include <wallLoad/core/vektor.hpp>
#include <boost/random.hpp>
#include <time.h>

namespace wallLoad {
    namespace core {
        class diffuseScatter {
            public:
                diffuseScatter() :
                    m_generator(time(0)),
                    m_distribution(-1.0, 1.0) {
                    }
                virtual ~diffuseScatter() {}

                vektor get_direction(const vektor & normal) {

                }

            protected:
                boost::random::mt19937 m_generator;
                boost::random::uniform_real_distribution<double> m_distribution;

        };
    }
}

#endif 
