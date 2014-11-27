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
        /*! \brief Probability density function class
         *
         * This class represents a probability density function.
         * It creates random numbers which occurrence is proportional to the specified PDF.
         */
        class probabilityDistribution {
            public:
                /*! \brief Python constructor
                 *
                 * This constructor initializes the probability density function with the x,y values given as python lists.
                 * This constructor is intended as interface to python.
                 * Do not use this function from within C++.
                 */
                probabilityDistribution(const boost::python::list & x, const boost::python::list & y) : 
                    m_generator(time(0)), N(boost::python::len(x)), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    for(uint32_t i = 0; i < boost::python::len(x); ++i) {
                        *(m_x + i) = boost::python::extract<double>(x[i]);
                        *(m_y + i) = boost::python::extract<double>(y[i]);
                    }
                    calculate_accumulated();
                }

                /*! \brief Copy constructor */
                probabilityDistribution(const probabilityDistribution & rhs) :
                    m_generator(time(0)), N(rhs.N),
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(rhs.m_x, rhs.m_x + N, m_x);
                    std::copy(rhs.m_y, rhs.m_y + N, m_y);
                    std::copy(rhs.m_accumulated, rhs.m_accumulated + N, m_accumulated);
                }

                
                /*! \brief Constructor
                 *
                 * This constructor initializes the probability density function with the x,y values.
                 */
                probabilityDistribution(const std::vector<double> & x, const std::vector<double> & y) :
                    m_generator(time(0)), N(x.size()), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(x.begin(), x.end(), m_x);
                    std::copy(y.begin(), y.end(), m_y);
                    calculate_accumulated();
                }

                /*! \brief Constructor
                 *
                 * This constructor initializes the probability density function with the x,y values.
                 */
                probabilityDistribution(const uint32_t n, const double * x, const double * y) :
                    m_generator(time(0)), N(n), 
                    m_x(new double[N]), m_y(new double[N]), m_accumulated(new double[N]) {
                    std::copy(x, x + N, m_x);
                    std::copy(y, y + N, m_y);
                    calculate_accumulated();
                }

                /*! \brief Destructor */
                virtual ~probabilityDistribution() {
                    delete [] m_x;
                    delete [] m_y;
                    delete [] m_accumulated;
                }

                /*! \brief Get points on x axis.
                 *
                 * Returns the values on the x axis of the probability distribution.
                 */
                inline std::vector<double> get_x() const {
                    return std::vector<double>(m_x, m_x + N);
                }

                /*! \brief Get points on y axis.
                 *
                 * Returns the values on the y axis of the probability distribution.
                 */
                inline std::vector<double> get_y() const {
                    return std::vector<double>(m_x, m_y + N);
                }

                /*! \brief Get the cumulated density function
                 *
                 * Returns the cumulated density function.
                 */
                inline std::vector<double> get_accumulated() const {
                    return std::vector<double>(m_accumulated, m_accumulated + N);
                }


                /*! \brief Get the cumulated density function as python list.
                 *
                 * Returns the cumulated density function as python list.
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_accumulated_python() const {
                    boost::python::list output;
                    for(double * iter = m_accumulated; iter != m_accumulated + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                /*! \brief Get points on x axis as python list.
                 *
                 * Returns the values on the x axis of the probability distribution as python list.
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_x_python() const {
                    boost::python::list output;
                    for(double * iter = m_x; iter != m_x + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                /*! \brief Get points on y axis as python list.
                 *
                 * Returns the values on the y axis of the probability distribution as python list.
                 * This function is intended as python interface.
                 * Do not use this function from within C++.
                 */
                boost::python::list get_y_python() const {
                    boost::python::list output;
                    for(double * iter = m_y; iter != m_y + N; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                /*! \brief Create a random number.
                 *
                 * This function returns a random number.
                 * The distribution of the random numbers is equal to the probability density function stored in the instance.
                 */
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

                /*! \brief Get the maximum probability density */
                double get_max() const { return *std::max_element(m_y, m_y+N); }

                /*! \brief Calculate the probability at the given point x.
                 *
                 * This function returns the linear interpolated probability density at the given point x.
                 */
                double get_value(const double x) const {
                    if((x < *m_x) || (x > *(m_x + N - 1))) return 0.0;
                    uint32_t i0 = 0;
                    for(uint32_t i = 1; i < N; ++i) {
                        if((m_x[i-1] <= x) && (m_x[i] >= x)) i0 = i;
                    }
                    double t = x - m_x[i0-1];
                    return (1.0 - t)*m_y[i0-1] + t*m_y[i0];
                }



            protected:
                /*! \brief Calculate the cumulated density function.
                 *
                 * This function calculates the cumulated density function for the given probability density function.
                 */
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

                boost::random::mt19937 m_generator; /*!< \brief Random number generator */
                boost::random::uniform_01<double> m_distribution; /*!< \brief Random uniform distribution \f$[0,1[\f$. */
                uint32_t N; /*!< \brief Number of points on the distribution. */
                double * m_x; /*!< \brief Points on the x axis of the distribution. */
                double * m_y; /*!< \brief Values of the probability distribution. */
                double * m_accumulated; /*!< Cumulated probability density function. */

        };
    }
}

#endif 
