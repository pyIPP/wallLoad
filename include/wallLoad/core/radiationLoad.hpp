#ifndef include_wallLoad_core_radiationLoad_hpp
#define include_wallLoad_core_radiationLoad_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <algorithm>
#include <wallLoad/core/mesh.hpp>
#include <wallLoad/core/hitResult.hpp>
#include <wallLoad/core/radiationDistribution.hpp>
#include <wallLoad/core/directionGenerator.hpp>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>

namespace wallLoad {
    namespace core {
        class radiationLoad : public std::vector<uint32_t> {
            public:
                radiationLoad(const mesh & grid, const radiationDistribution & distribution) :
                    std::vector<uint32_t>(grid.size(), 0),
                    m_mesh(grid), m_radiationDistribution(distribution), m_directionGenerator(),
                    m_generator(time(0)), m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }
                radiationLoad(const radiationLoad & rhs) :
                    std::vector<uint32_t>(rhs),
                    m_mesh(rhs.m_mesh), m_radiationDistribution(rhs.m_radiationDistribution),
                    m_directionGenerator(), m_generator(time(0)), 
                    m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }

                virtual ~radiationLoad() {
                }

                radiationLoad & operator=(const radiationLoad & rhs) {
                    if(this != &rhs) {
                        std::vector<uint32_t>::operator=(rhs);
                        m_mesh = rhs.m_mesh;
                        m_radiationDistribution = rhs.m_radiationDistribution;
                    }
                    return *this;
                }

                void clear() {
                    std::fill(begin(), end(), 0);
                }

                void add_samples(const uint32_t N) {
                    hitResult temp;
                    for(uint32_t i = 0; i < N; ) {
                        temp = m_mesh.evaluateHit(m_radiationDistribution.get_random_toroidal_point(), m_directionGenerator.generate());
                        if(temp) {
                            ++std::vector<uint32_t>::operator[](temp.get_element());
                            ++i;
                        }
                    }
                }

                uint32_t operator[] (const uint32_t i) const {
                    return std::vector<uint32_t>::operator[](i);
                }

                uint32_t get_total_hits() const {
                    return std::accumulate(begin(), end(), 0);
                }

                std::vector<double> get_heat_flux(const double Ptot) const {
                    uint32_t hits = get_total_hits();
                    std::vector<double> areas = m_mesh.get_areas();
                    std::vector<double>::const_iterator area = areas.begin();
                    std::vector<double> output;
                    for(auto iter = begin(); iter != end(); ++iter, ++area) {
                        output.push_back((double)*iter/hits/(*area)*Ptot);
                    }
                    return output;
                }


                boost::python::list get_heat_flux_python(const double Ptot) const {
                    boost::python::list output;
                    std::vector<double> heatFlux = get_heat_flux(Ptot);
                    for(auto iter = heatFlux.begin(); iter != heatFlux.end(); ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                mesh get_mesh() const {
                    return m_mesh;
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
