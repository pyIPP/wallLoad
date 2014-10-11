#ifndef include_probabilityDistribution_hpp
#define include_probabilityDistribution_hpp

#include <boost/python.hpp>
#include <vector>
#include <stdint.h>
#include <boost/random.hpp>
#include <time.h>
#include <algorithm>

class probabilityDistribution
{
    public:
        probabilityDistribution(const boost::python::list & x, const boost::python::list & y) : m_generator(time(0)) {
            std::vector<double> xTemp, yTemp;
            for(uint32_t i = 0; i < boost::python::len(x); ++i) {
                xTemp.push_back(boost::python::extract<double>(x[i]));
                yTemp.push_back(boost::python::extract<double>(y[i]));
            }
            set_distribution(xTemp, yTemp);
        }
        probabilityDistribution(const probabilityDistribution & rhs) : m_generator(time(0)), 
            m_x(rhs.m_x), m_y(rhs.m_y), m_accumulated(rhs.m_accumulated) {
        }

        virtual ~probabilityDistribution() {}

        boost::python::list get_accumulated() const {
            boost::python::list output;
            for(uint32_t i = 0; i < m_accumulated.size(); ++i) {
                output.append(m_accumulated[i]);
            }
            return output;
        }

        boost::python::list get_x() const {
            boost::python::list output;
            for(uint32_t i = 0; i < m_x.size(); ++i) {
                output.append(m_x[i]);
            }
            return output;
        }

        boost::python::list get_y() const {
            boost::python::list output;
            for(uint32_t i = 0; i < m_y.size(); ++i) {
                output.append(m_y[i]);
            }
            return output;
        }

        double get_random_number() {
            double temp = m_distribution(m_generator);
            for(uint32_t i = 1; i < m_accumulated.size(); ++i) {
                if( (temp >= m_accumulated[i-1]) && (temp < m_accumulated[i]) ) {
                    temp = (m_accumulated[i]-temp)/(m_accumulated[i]-m_accumulated[i-1]);
                    return (1.0-temp)*m_x[i-1] + temp*m_x[i];
                }
            }
            return 0.0;
        }

        double get_max() { return *std::max_element(m_y.begin(), m_y.end()); }

        double get_value(const double x) {
            if((x < *m_x.begin()) || (x > *m_x.rbegin())) return 0.0;
            uint32_t i0 = 0;
            for(uint32_t i = 1; i < m_x.size(); ++i) {
                if((m_x[i-1] <= x) && (m_x[i] >= x)) i0 = i;
            }
            double t = x - m_x[i0-1];
            return (1.0 - t)*m_y[i0-1] + t*m_y[i0];
        }



    protected:
        void set_distribution(const std::vector<double> & x, const std::vector<double> & y) {
            m_x = x;
            m_y = y;
            m_accumulated.resize(x.size());
            for(uint32_t i = 1; i < x.size(); ++i) {
                m_accumulated[i] = m_accumulated[i-1] + (m_y[i] + m_y[i-1])/2.0*(m_x[i]-m_x[i-1]);
            }
            double norm = m_accumulated[x.size()-1];
            for(uint32_t i = 1; i < x.size(); ++i) {
                m_accumulated[i] /= norm;
            }
        }
        boost::random::mt19937 m_generator;
        boost::random::uniform_01<double> m_distribution;

        std::vector<double> m_x;
        std::vector<double> m_y;
        std::vector<double> m_accumulated;

};





#endif 
