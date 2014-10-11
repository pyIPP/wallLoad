#ifndef include_radiationProfile_hpp
#define include_radiationProfile_hpp

#include <boost/python.hpp>
#include <stdint.h>
#include <probabilityDistribution.hpp>


class radiationProfile {
    public:
        radiationProfile(const boost::python::list & rho, const boost::python::list & powerDensity) {
            N = boost::python::len(rho);
            m_rho = new double[N];
            m_powerDensity = new double[N];
            for(uint32_t i = 0; i < N; ++i) {
                *(m_rho + i) = boost::python::extract<double>(rho[i]);
                *(m_powerDensity + i) = boost::python::extract<double>(powerDensity[i]);
            }
        }

        radiationProfile(const radiationProfile & rhs) : N(rhs.N), m_rho(new double[N]), m_powerDensity(new double [N]) {
            std::copy(rhs.m_rho, rhs.m_rho + N, m_rho);
            std::copy(rhs.m_powerDensity, rhs.m_powerDensity + N, m_powerDensity);
        }

        virtual ~radiationProfile() {
            delete [] m_rho;
            delete [] m_powerDensity;
        }

        radiationProfile & operator=(const radiationProfile & rhs) {
            if(this != &rhs) {
                N = rhs.N;
                std::copy(rhs.m_rho, rhs.m_rho + N, m_rho);
                std::copy(rhs.m_powerDensity, rhs.m_powerDensity + N, m_powerDensity);
            }
            return *this;
        }

        boost::python::list get_rho() const { 
            boost::python::list output;
            for(double * temp = m_rho; temp != m_rho + N; ++temp) {
                output.append(*temp);
            }
            return output;
        }
        boost::python::list get_powerDensity() const { 
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
            boost::python::list rho, probability;
            for(uint32_t i = 0; i < N; ++i) {
                rho.append(*(m_rho + i));
                probability.append(*(m_powerDensity + i)/integral);
            }
            return probabilityDistribution(rho, probability);
        }



    protected:
        uint32_t N;
        double * m_rho;
        double * m_powerDensity;
};



#endif
