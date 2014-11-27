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
        /*! \brief Random direction generator for diffuse scatter.
         *
         * Provided a normal vector this class generates random direction vectors for diffuse scatter.
         */
        class diffuseScatter {
            public:
                /*! \brief Constructor */
                diffuseScatter() :
                    m_generator(time(0)),
                    m_one(),
                    m_2pi_distribution(0, 2.0*boost::math::constants::pi<double>()) {
                    }
                /*! \brief Destructor */
                virtual ~diffuseScatter() {}

                /*! \brief Generate random direction vector.
                 *
                 * This function returns a random direction vector for diffuse scatter.
                 * \param normal Normal vector of the surface the ray is scattered on.
                 */
                vektor get_direction(const vektor & normal) {
                    double alpha = m_2pi_distribution(m_generator);
                    double beta = asin(m_one(m_generator));
                    vektor candidate(sin(beta)*cos(alpha), sin(beta)*sin(alpha), cos(beta));

                    return candidate;
                }

            protected:
                boost::random::mt19937 m_generator; /*!< \brief Random number generator */
                boost::random::uniform_01<double> m_one; /*!< \brief Random uniform distribution \f$[0,1[\f$. */
                boost::random::uniform_real_distribution<double> m_2pi_distribution; /*!< \brief Random number distribution \f$[0,2\pi[\f$. */

        };
    }
}

#endif 
