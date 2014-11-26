#ifndef include_wallLoad_core_radiationProfile_hpp
#define include_wallLoad_core_radiationProfile_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <wallLoad/core/probabilityDistribution.hpp>

namespace wallLoad {
    namespace core {
        /*! \brief Class representing a radial radiation profile.
         *
         * This class represents the radial radiation profile of the plasma.
         */
        class radiationProfile {
            public:
                /*! \brief Python constructor
                 *
                 * This constructor initializes the radiation profile with the given power densities and \f$\rho_{pol}\f$ values.
                 * This constructor is intended as python interface.
                 * Do not use this constructor from within C++.
                 */
                radiationProfile(const boost::python::list & rho, const boost::python::list & powerDensity) :
                    N(boost::python::len(rho)), m_rho(new double[N]), m_powerDensity(new double[N]) {
                    for(uint32_t i = 0; i < N; ++i) {
                        *(m_rho + i) = boost::python::extract<double>(rho[i]);
                        *(m_powerDensity + i) = boost::python::extract<double>(powerDensity[i]);
                    }
                }

                /*! \brief Constructor
                 *
                 * This constructor initializes the radiation profile with the given power densities and \f$\rho_{pol}\f$ values.
                 */
                radiationProfile(const std::vector<double> & rho, const std::vector<double> & powerDensity) :
                    N(rho.size()), m_rho(new double[N]), m_powerDensity(new double[N]) {
                    std::copy(rho.begin(), rho.end(), m_rho);
                    std::copy(powerDensity.begin(), powerDensity.end(), m_powerDensity);
                }

                /*! \brief Copy constructor */
                radiationProfile(const radiationProfile & rhs) : 
                    N(rhs.N), m_rho(new double[N]), m_powerDensity(new double[N]) {
                    std::copy(rhs.m_rho, rhs.m_rho+N, m_rho);
                    std::copy(rhs.m_powerDensity, rhs.m_powerDensity+N, m_powerDensity);
                }

                /*! \brief Destructor */
                virtual ~radiationProfile() {
                    delete [] m_rho;
                    delete [] m_powerDensity;
                }

                /*! \brief Assignment operator
                 * 
                 * This operator copies the radiation profile from the given instance to the current instance.
                 */
                radiationProfile & operator=(const radiationProfile & rhs) {
                    if(this != &rhs) {
                        N = rhs.N;
                        delete [] m_rho;
                        delete [] m_powerDensity;
                        m_rho = new double[N];
                        m_powerDensity = new double[N];
                        std::copy(rhs.m_rho, rhs.m_rho + N, m_rho);
                        std::copy(rhs.m_powerDensity, rhs.m_powerDensity + N, m_powerDensity);
                    }
                    return *this;
                }

                /*! \brief Get the \f$\rho_{pol}\f$ values.
                 *
                 * This function returns the \f$rho_{pol}\f$ values of the radiation profile.
                 */
                std::vector<double> get_rho() const {
                    return std::vector<double>(m_rho, m_rho + N);
                }

                /*! \brief Get the \f$\rho_{pol}\f$ values as python list.
                 *
                 * This function returns the \f$rho_{pol}\f$ values of the radiation profile.
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_rho_python() const { 
                    boost::python::list output;
                    for(double * temp = m_rho; temp != m_rho + N; ++temp) {
                        output.append(*temp);
                    }
                    return output;
                }

                /*! \brief Get the power densities.
                 *
                 * This function returns the power densities of the radiation profile.
                 */
                std::vector<double> get_powerDensity() const {
                    return std::vector<double>(m_powerDensity, m_powerDensity + N);
                }

                /*! \brief Get the power densities as python list.
                 *
                 * This function returns the power densities of the radiation profile.
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_powerDensity_python() const { 
                    boost::python::list output;
                    for(double * temp = m_powerDensity; temp != m_powerDensity + N; ++temp) {
                        output.append(*temp);
                    }
                    return output;
                }
               
                /*! \brief Get the probability distribution of the radiation profile.
                 *
                 * This function returns the probability distribution calculated from the radiation profile.
                 * This is necessary for the Monte Carlo calculation of the heat flux distribution.
                 */
                probabilityDistribution get_probabilityDistribution() const {
                    double integral = 0.0;
                    for(uint32_t i = 0; i < N-1; ++i) {
                        integral += 0.5*(*(m_powerDensity + i) + *(m_powerDensity + i + 1))*(*(m_rho + i + 1) - *(m_rho + i));
                    }
                    std::vector<double> rho = get_rho();
                    std::vector<double> powerDensity = get_powerDensity();
                    for(auto iter = powerDensity.begin(); iter != powerDensity.end(); ++iter) {
                        *iter /= integral;
                    }
                    return probabilityDistribution(rho, powerDensity);
                }

            protected:
                uint32_t N; /*!< \brief Number of points on the radiation profile. */
                double * m_rho; /*!< \brief \f$\rho_{pol}\f$ values of the radiation profile. */
                double * m_powerDensity; /*!< \brief Power densities of the radiation profile. */
        };
    }
}

#endif
