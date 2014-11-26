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
        /*! \brief Class to calculate and store the radiation load onto the first wall.
         *
         * This class calculates the radiation load onto the first wall for a given mesh and radiation distribution.
         * It is derived from std::vector to store the number of hits for each mesh element.
         * The calculation is done using a Monte Carlo approach.
         */
        class radiationLoad : public std::vector<uint32_t> {
            public:
                /*! \brief Constructor 
                 *
                 * This constructor initializes the class with the given mesh and radiation distribution.
                 */
                radiationLoad(const mesh & grid, const radiationDistribution & distribution) :
                    std::vector<uint32_t>(grid.size(), 0),
                    m_mesh(grid), m_radiationDistribution(distribution), m_directionGenerator(),
                    m_generator(time(0)), m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }
                /*! \brief Copy constructor */
                radiationLoad(const radiationLoad & rhs) :
                    std::vector<uint32_t>(rhs),
                    m_mesh(rhs.m_mesh), m_radiationDistribution(rhs.m_radiationDistribution),
                    m_directionGenerator(), m_generator(time(0)), 
                    m_2pi_distribution(0.0, 2.0*boost::math::constants::pi<double>()) {
                }

                /*! \brief Destructor */
                virtual ~radiationLoad() {
                }

                /*! Assignment operator
                 *
                 * This operator copies the information from the given radiation load to the current instance.
                 */
                radiationLoad & operator=(const radiationLoad & rhs) {
                    if(this != &rhs) {
                        std::vector<uint32_t>::operator=(rhs);
                        m_mesh = rhs.m_mesh;
                        m_radiationDistribution = rhs.m_radiationDistribution;
                    }
                    return *this;
                }

                /*! \brief Clear the recorded hits.
                 *
                 * This function sets the recorded hits of all mesh elements to zero.
                 */
                void clear() {
                    std::fill(begin(), end(), 0);
                }

                /*! \brief Add samples
                 *
                 * This function calculates the hits for N random samples.
                 */
                void add_samples(const uint32_t N) {
                    hitResult temp;
                    for(uint32_t i = 0; i < N; ) {
                        temp = m_mesh.evaluateHit(m_radiationDistribution.get_random_toroidal_point(), m_directionGenerator.generate());
                        if(temp) {
                            ++std::vector<uint32_t>::operator[](temp.element);
                            ++i;
                        }
                    }
                }

                /*! \brief Get number of hits for ith element. */
                uint32_t operator[] (const uint32_t i) const {
                    return std::vector<uint32_t>::operator[](i);
                }

                /*! \brief Get total number of hits
                 *
                 * This function returns the total number of hits recorded.
                 * \f$N = \sum\limits N_i\f$
                 */
                uint32_t get_total_hits() const {
                    return std::accumulate(begin(), end(), 0);
                }

                /*! \brief Calculate the heat flux density onto mesh elements
                 *
                 * Provided the total power, this function calculates the heat flux density onto the different mesh elements.
                 * \f$ P_i = P_{tot} \frac{N_i}{N A_i} \f$
                 */
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

                /*! \brief Calculate the heat flux density onto mesh elements and return them as python list
                 *
                 * Provided the total power, this function calculates the heat flux density onto the different mesh elements.
                 * \f$ P_i = P_{tot} \frac{N_i}{N A_i} \f$
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_heat_flux_python(const double Ptot) const {
                    boost::python::list output;
                    std::vector<double> heatFlux = get_heat_flux(Ptot);
                    for(auto iter = heatFlux.begin(); iter != heatFlux.end(); ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                /*! \brief Get the mesh
                 *
                 * This function returns the mesh of the first wall.
                 */
                mesh get_mesh() const {
                    return m_mesh;
                }

            protected:
                mesh m_mesh; /*!< \brief Mesh representing the first wall. */
                radiationDistribution m_radiationDistribution; /*!< \brief Assumed radiation distribution of the plasma. */
                directionGenerator m_directionGenerator; /*!< \brief Generator for random direction vectors. */
                boost::random::mt19937 m_generator; /*!< \brief Random number generator for the Monte Carlo calculation */
                boost::random::uniform_real_distribution<double> m_2pi_distribution; /*!< \brief Uniform random distribution \f$\left[0,2\pi\right[\f$. */

        };
    }
}


#endif 
