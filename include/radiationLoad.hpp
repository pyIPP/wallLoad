#ifndef include_radiationLoad_hpp
#define include_radiationLoad_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <mesh.hpp>
#include <hitResult.hpp>
#include <radiationDistribution.hpp>
#include <directionGenerator.hpp>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>

class radiationLoad {
    public:
        radiationLoad(const mesh & grid, const radiationDistribution & distribution) 
            : m_mesh(grid), m_radiationDistribution(distribution), m_directionGenerator(),
            m_generator(time(0)), m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
        }
        virtual ~radiationLoad() {
        }
        boost::python::list get_samples(const uint32_t N) {
            boost::python::list points = m_radiationDistribution.get_random_toroidal_points(N);
            boost::python::list direction = m_directionGenerator.generate(N);
            boost::python::list output = m_mesh.evaluateHits(points, direction);
            return output;
        }

    protected:
        mesh m_mesh;
        radiationDistribution m_radiationDistribution;
        directionGenerator m_directionGenerator;
        boost::random::mt19937 m_generator;
        boost::random::uniform_real_distribution<double> m_2pi_distribution;

};


#endif 
