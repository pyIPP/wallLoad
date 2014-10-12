#ifndef include_wallLoad_core_directionGenerator_hpp
#define include_wallLoad_core_directionGenerator_hpp

#include <boost/python.hpp>
#include <time.h>
#include <math.h>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>
#include <wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        class directionGenerator {
            public:
                directionGenerator() : 
                    m_generator(time(0)),
                    m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()), 
                    m_pi_distribution(0.0, boost::math::constants::pi<double>()) {
                }
                virtual ~directionGenerator() {}

                inline vektor generate() {
                    double alpha = m_2pi_distribution(m_generator);
                    double beta = m_pi_distribution(m_generator);
                    return vektor(sin(beta)*cos(alpha), sin(beta)*sin(alpha), cos(beta));
                }
                inline std::vector<vektor> generate(const uint32_t N) {
                    std::vector<vektor> output;
                    for(uint32_t i = 0; i < N; ++i) {
                        output.push_back(generate());
                    }
                    return output;
                }

                boost::python::list generate_python(const unsigned int N) {
                    boost::python::list list;
                    std::vector<vektor> temp = generate(N);
                    for(auto iter = temp.begin(); iter != temp.end(); ++iter) {
                        list.append(*iter);
                    }
                    return list;
                }
            protected:
                boost::random::mt19937 m_generator;
                boost::random::uniform_real_distribution<double> m_2pi_distribution;
                boost::random::uniform_real_distribution<double> m_pi_distribution;
        };    
    }
}

#endif 
