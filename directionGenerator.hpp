#ifndef package_directionGenerator_hpp
#define package_directionGenerator_hpp

#include <boost/python.hpp>
#include <boost/numpy.hpp>
#include <time.h>
#include <cmath>
#include <boost/random.hpp>

class directionGenerator {
    public:
        directionGenerator() : 
            m_generator(time(0)),
            m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()), 
            m_pi_distribution(0.0, boost::math::constants::pi<double>()),
            m_2pi_generator(m_generator, m_2pi_distribution), 
            m_pi_generator(m_generator, m_pi_distribution) {
        }
        virtual ~directionGenerator() {}

        boost::python::list generate(const unsigned int N) {
            boost::python::list list;
            double alpha, beta;
            for(uint32_t i = 0; i < N; ++i) {
                alpha = m_2pi_generator();
                beta = m_pi_generator();
                list.append(vektor(std::sin(beta)*std::cos(alpha), std::sin(beta)*std::sin(alpha), std::cos(beta)));
            }
            return list;
        }
    protected:
        boost::random::mt19937 m_generator;
        boost::random::uniform_real_distribution<double> m_2pi_distribution;
        boost::random::uniform_real_distribution<double> m_pi_distribution;
        boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution <double> > m_2pi_generator;
        boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution <double> > m_pi_generator;


};    



#endif 
