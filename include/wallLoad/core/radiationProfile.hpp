#ifndef include_wallLoad_core_radiationProfile_hpp
#define include_wallLoad_core_radiationProfile_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <wallLoad/core/probabilityDistribution.hpp>

namespace wallLoad {
    namespace core {
        class radiationProfile {
            public:
                radiationProfile(const boost::python::list & rho, const boost::python::list & powerDensity) :
                    N(boost::python::len(rho)), m_rho(new double[N]), m_powerDensity(new double[N]) {
                    for(uint32_t i = 0; i < N; ++i) {
                        *(m_rho + i) = boost::python::extract<double>(rho[i]);
                        *(m_powerDensity + i) = boost::python::extract<double>(powerDensity[i]);
                    }
                }

                radiationProfile(const std::vector<double> & rho, const std::vector<double> & powerDensity) :
                    N(rho.size()), m_rho(new double[N]), m_powerDensity(new double[N]) {
                    std::copy(rho.begin(), rho.end(), m_rho);
                    std::copy(powerDensity.begin(), powerDensity.end(), m_powerDensity);
                }

                virtual ~radiationProfile() {
                    delete [] m_rho;
                    delete [] m_powerDensity;
                }

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

                std::vector<double> get_rho() const {
                    return std::vector<double>(m_rho, m_rho + N);
                }

                boost::python::list get_rho_python() const { 
                    boost::python::list output;
                    for(double * temp = m_rho; temp != m_rho + N; ++temp) {
                        output.append(*temp);
                    }
                    return output;
                }

                std::vector<double> get_powerDensity() const {
                    return std::vector<double>(m_powerDensity, m_powerDensity + N);
                }

                boost::python::list get_powerDensity_python() const { 
                    boost::python::list output;
                    for(double * temp = m_powerDensity; temp != m_powerDensity + N; ++temp) {
                        output.append(*temp);
                    }
                    return output;
                }
                
                probabilityDistribution get_probabilityDistribution() const {
                    double integral = 0.0;
                    for(uint32_t i = 0; i < N-1; ++i) {
                        integral += 0.5*(*(m_powerDensity + i) + *(m_powerDensity + i + 1))*(*(m_rho + i + 1) - *(m_rho + i));
                    }
                    double * x = new double[N];
                    double * y = new double[N];
                    double * iter = y;
                    double * power = m_powerDensity;
                    std::copy(m_rho, m_rho + N, x);
                    for(; iter != y + N; ++iter, ++power) {
                        *iter = *power/integral;
                    }
                    probabilityDistribution output(N, x, y);
                    delete [] x;
                    delete [] y;
                    return output;
                }

            protected:
                uint32_t N;
                double * m_rho;
                double * m_powerDensity;
        };
    }
}

#endif
