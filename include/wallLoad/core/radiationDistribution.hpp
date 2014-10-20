#ifndef include_wallLoad_core_radiationDistribution_hpp
#define include_wallLoad_core_radiationDistribution_hpp

#include <boost/python.hpp>
#include <wallLoad/core/equilibrium.hpp>
#include <wallLoad/core/vektor.hpp>
#include <wallLoad/core/radiationProfile.hpp>
#include <wallLoad/core/polygon.hpp>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>
#include <math.h>

namespace wallLoad {
    namespace core {
        class radiationDistribution {
            public:
                radiationDistribution(const equilibrium & equi, const radiationProfile & profile) : 
                    m_equilibrium(equi), m_profile(profile), 
                    m_radiationProbability(profile.get_probabilityDistribution()),
                    m_generator(time(0)),
                    m_R(equi.get_Rmin(), equi.get_Rmax()),
                    m_z(equi.get_zmin(), equi.get_zmax()),
                    m_2pi(0.0, 2.0*boost::math::constants::pi<double>()),
                    m_hasContour(false),
                    m_contour()
                    {
                }

                radiationDistribution(const equilibrium & equi, const radiationProfile & profile, const polygon & contour) : 
                    m_equilibrium(equi), m_profile(profile), 
                    m_radiationProbability(profile.get_probabilityDistribution()),
                    m_generator(time(0)),
                    m_R(equi.get_Rmin(), equi.get_Rmax()),
                    m_z(equi.get_zmin(), equi.get_zmax()),
                    m_2pi(0.0, 2.0*boost::math::constants::pi<double>()),
                    m_hasContour(true),
                    m_contour(contour)
                    {
                }

                virtual ~radiationDistribution() {}

                std::vector<vektor> get_random_points(const uint32_t N = 1) {
                    std::vector<vektor> output;
                    double R, z, u, P, rho;
                    double M = m_radiationProbability.get_max();
                    double R0 = m_equilibrium.get_R0();
                    while(output.size() < N) {
                        R = m_R(m_generator);
                        z = m_z(m_generator);
                        u = m_u(m_generator);
                        rho = m_equilibrium.get_rho(R,z);
                        P = m_radiationProbability.get_value(rho)*R;
                        if(m_hasContour && !m_contour.inside(R,z)) {
                            P = 0.0;
                        }
                        if( u < P/R0/M ) {
                            output.push_back(vektor(R,0,z));
                        }
                    }
                    return output;
                }

                boost::python::list get_random_points_python(const uint32_t N = 1) {
                    std::vector<vektor> temp = get_random_points(N);
                    boost::python::list output;
                    for(auto iter = temp.begin(); iter != temp.end(); ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                inline vektor get_random_toroidal_point() {
                    double R, z, u, P, rho;
                    double M = m_radiationProbability.get_max();
                    double R0 = m_equilibrium.get_R0();
                    double alpha;
                    R = m_R(m_generator);
                    z = m_z(m_generator);
                    u = m_u(m_generator);
                    rho = m_equilibrium.get_rho(R,z);
                    P = m_radiationProbability.get_value(rho)*R;
                    if(m_hasContour && !m_contour.inside(R,z)) {
                        P = 0.0;
                    }
                    if( u < P/R0/M ) {
                        alpha = m_2pi(m_generator);
                        return vektor(R*cos(alpha),R*sin(alpha),z);
                    }
                    return get_random_toroidal_point();
                }

                std::vector<vektor> get_random_toroidal_points(const uint32_t N = 1) {
                    std::vector<vektor> output;
                    double R, z, u, P, rho;
                    double M = m_radiationProbability.get_max();
                    double R0 = m_equilibrium.get_R0();
                    double alpha, sina, cosa;
                    while(output.size() < N) {
                        R = m_R(m_generator);
                        z = m_z(m_generator);
                        u = m_u(m_generator);
                        rho = m_equilibrium.get_rho(R,z);
                        P = m_radiationProbability.get_value(rho)*R;
                        if(m_hasContour && !m_contour.inside(R,z)) {
                            P = 0.0;
                        }
                        if( u < P/R0/M ) {
                            alpha = m_2pi(m_generator);
                            cosa = cos(alpha);
                            sina = sin(alpha);
                            output.push_back(vektor(R*cosa,R*sina,z));
                        }
                    }
                    return output;
                }

                boost::python::list get_random_toroidal_points_python(const uint32_t N = 1) {
                    std::vector<vektor> temp = get_random_toroidal_points(N);
                    boost::python::list output;
                    for(auto iter = temp.begin(); iter != temp.end(); ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                void set_Rmin(const double Rmin) {
                    m_R.param(boost::random::uniform_real_distribution<double>::param_type(Rmin, m_R.param().b()));
                }
                void set_Rmax(const double Rmax) {
                    m_R.param(boost::random::uniform_real_distribution<double>::param_type(m_R.param().a(), Rmax));
                }
                void set_zmin(double zmin) {
                    m_z.param(boost::random::uniform_real_distribution<double>::param_type(zmin, m_z.param().b()));
                }
                void set_zmax(const double zmax) {
                    m_z.param(boost::random::uniform_real_distribution<double>::param_type(m_z.param().a(), zmax));
                }
                double get_Rmin() const {
                    return m_R.param().a();
                }
                double get_Rmax() const {
                    return m_R.param().b();
                }
                double get_zmin() const {
                    return m_z.param().a();
                }
                double get_zmax() const {
                    return m_z.param().b();
                }

            protected:
                equilibrium m_equilibrium;
                radiationProfile m_profile;
                probabilityDistribution m_radiationProbability;        
                boost::random::mt19937 m_generator;
                boost::random::uniform_real_distribution<double> m_R;
                boost::random::uniform_real_distribution<double> m_z;
                boost::random::uniform_01<double> m_u;
                boost::random::uniform_real_distribution<double> m_2pi;
                bool m_hasContour;
                polygon m_contour;
        };
    }
}

#endif
