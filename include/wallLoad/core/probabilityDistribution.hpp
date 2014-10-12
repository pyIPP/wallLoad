#ifndef include_wallLoad_core_probabilityDistribution_hpp
#define include_wallLoad_core_probabilityDistribution_hpp

#include <boost/python.hpp>
#include <vector>
#include <stdint.h>
#include <boost/random.hpp>
#include <time.h>
#include <algorithm>

namespace wallLoad {
    namespace core {
        class probabilityDistribution
        {
            public:
                probabilityDistribution(const boost::python::list & x, const boost::python::list & y) : 
                    m_generator(time(0)), N(boost::python::len(x)), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    for(uint32_t i = 0; i < boost::python::len(x); ++i) {
                        *(m_x + i) = boost::python::extract<double>(x[i]);
                        *(m_y + i) = boost::python::extract<double>(y[i]);
                    }
                    calculate_accumulated();
                }

                probabilityDistribution(const probabilityDistribution & rhs) :
                    m_generator(time(0)), N(rhs.N),
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(rhs.m_x, rhs.m_x + N, m_x);
                    std::copy(rhs.m_y, rhs.m_y + N, m_y);
                    std::copy(rhs.m_accumulated, rhs.m_accumulated + N, m_accumulated);
                }

                probabilityDistribution(const std::vector<double> & x, const std::vector<double> & y) :
                    m_generator(time(0)), N(x.size()), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(x.begin(), x.end(), m_x);
                    std::copy(y.begin(), y.end(), m_y);
                    calculate_accumulated();
                }

                probabilityDistribution(const uint32_t n, const double * x, const double * y) {
                    m_generator(time(0)), N(m), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(x, x + N, m_x);
                    std::copy(y, y + N, m_y);
                    calculate_accumulated();
                }

                virtual ~probabilityDistribution() {
                    delete [] m_x;
                    delete [] m_y;
                    delete [] m_accumulated;
                }

                inline std::vector<double> get_x() const {
                    return std::vector<double>(m_x, m_x + N);
                }

                inline std::vector<double> get_y() const {
                    return std::vector<double>(m_x, m_y + N);
                }

                inline std::vector<double> get_accumulated() const {
                    return std::vector<double>(m_accumulated, m_accumulated + N);
                }


                boost::python::list get_accumulated() const {
                    boost::python::list output;
                    for(double * iter = m_accumulated; iter != m_accumulated + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                boost::python::list get_x_python() const {
                    boost::python::list output;
                    for(double * iter = m_x; iter != m_x + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                boost::python::list get_y_python() const {
                    boost::python::list output;
                    for(double * iter = m_y; iter != m_y + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                double get_random_number() {
                    double temp = m_distribution(m_generator);
                    for(uint32_t i = 1; i < N; ++i) {
                        if( (temp >= m_accumulated[i-1]) && (temp <= m_accumulated[i]) ) {
                            temp = (m_accumulated[i]-temp)/(m_accumulated[i]-m_accumulated[i-1]);
                            return (1.0-temp)*m_x[i-1] + temp*m_x[i];
                        }
                    }
                    return 0.0;
                }

                double get_max() { return *std::max_element(m_y, m_y+N); }

                double get_value(const double x) {
                    if((x < *m_x) || (x > *(m_x + N - 1))) return 0.0;
                    uint32_t i0 = 0;
                    for(uint32_t i = 1; i < m_x.size(); ++i) {
                        if((m_x[i-1] <= x) && (m_x[i] >= x)) i0 = i;
                    }
                    double t = x - m_x[i0-1];
                    return (1.0 - t)*m_y[i0-1] + t*m_y[i0];
                }



            protected:
                void calculate_accumulated() {
                    *m_accumulated = 0;
                    for(uint32_t i = 1; i < N; ++i) {
                        m_accumulated[i] = m_accumulated[i-1] + (m_y[i] + m_y[i-1])/2.0*(m_x[i]-m_x[i-1]);
                    }
                    double norm = m_accumulated[N-1];
                    for(uint32_t i = 1; i < N; ++i) {
                        m_accumulated[i] /= norm;
                    }
                }

                boost::random::mt19937 m_generator;
                boost::random::uniform_01<double> m_distribution;
                uint32_t N;
                double m_x;
                double m_y;
                double m_accumulated;

        };
    }
}

#endif 
