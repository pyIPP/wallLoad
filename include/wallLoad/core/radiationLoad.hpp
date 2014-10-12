#ifndef include_wallLoad_core_radiationLoad_hpp
#define include_wallLoad_core_radiationLoad_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <wallLoad/core/mesh.hpp>
#include <wallLoad/core/hitResult.hpp>
#include <wallLoad/core/radiationDistribution.hpp>
#include <wallLoad/core/directionGenerator.hpp>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>

namespace wallLoad {
    namespace core {
        class radiationLoad {
            public:
                radiationLoad(const mesh & grid, const radiationDistribution & distribution) 
                    : m_mesh(grid), m_radiationDistribution(distribution), m_directionGenerator(),
                    m_generator(time(0)), m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }
                radiationLoad(const radiationLoad & rhs) :
                    m_mesh(rhs.m_mesh), m_radiationDistribution(rhs.m_radiationDistribution),
                    m_directionGenerator(), m_generator(time(0)), 
                    m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }

                virtual ~radiationLoad() {
                }

                radiationLoad & operator=(const radiationLoad & rhs) {
                    if(this != &rhs) {
                        m_mesh = rhs.m_mesh;
                        m_radiationDistribution = rhs.m_radiationDistribution;
                    }
                    return *this;
                }

                std::vector<hitResult> get_samples(const uint32_t N) {
                    std::vector<vektor> points = m_radiationDistribution.get_random_toroidal_points(N);
                    std::vector<vektor> direction = m_directionGenerator.generate(N);
                    return m_mesh.evaluateHits(points, direction);
                }

            protected:
                mesh m_mesh;
                radiationDistribution m_radiationDistribution;
                directionGenerator m_directionGenerator;
                boost::random::mt19937 m_generator;
                boost::random::uniform_real_distribution<double> m_2pi_distribution;

        };
    }
}


#endif 
