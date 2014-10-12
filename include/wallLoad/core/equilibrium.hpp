#ifndef include_wallLoad_core_equilibrium_hpp
#define include_wallLoad_core_equilibrium_hpp

#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <stdint.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <algorithm>

namespace wallLoad {
    namespace core {
        class equilibrium {
            public: 
                equilibrium(const std::string & filename) {
                    std::ifstream file(filename.c_str(), std::ios::in);
                    uint32_t temp;
                    double dTemp;
                    file >> m_comment >> temp >> m_NR >> m_Nz;
                    m_R = new double[m_NR];
                    m_z = new double[m_Nz];
                    file >> m_rBoxLength >> m_zBoxLength >> m_r0Exp >> m_rBoxLeft >> m_zBoxMid;
                    m_R = new double[m_NR];
                    m_z = new double[m_Nz];
                    m_psi = new double[m_NR*m_Nz];
                    double dR = m_rBoxLength/(m_NR-1);
                    for(uint32_t i = 0; i < m_NR; ++i){ 
                        *(m_R + i) = m_rBoxLeft + dR * i;
                    }
                    double dz = m_zBoxLength/(m_Nz-1);
                    for(uint32_t i = 0; i < m_Nz; ++i) {
                        *(m_z + i) = m_zBoxMid - m_zBoxLength/2.0 + dz * i;
                    }
                    file >> m_R0 >> m_z0 >> m_psiAxis >> m_psiEdge >> m_Btor;
                    file >> m_Ip >> dTemp >> dTemp >> dTemp >> dTemp;
                    file >> dTemp >> dTemp >> dTemp >> dTemp >> dTemp;
                    for(uint32_t i = 0; i < m_NR*4; ++i) {
                        file >> dTemp;
                    }
                    dTemp = 0;
                    for(uint32_t i = 0; i < m_NR*m_Nz; ++i) {
                        file >> *(m_psi + i);
                        *(m_psi + i) *= -1.0;
                        if(*(m_psi + i) < dTemp) dTemp = *(m_psi + i);
                    }

                }

                equilibrium(const equilibrium & rhs) : m_comment(rhs.m_comment), m_NR(rhs.m_NR), m_Nz(rhs.m_Nz),
                    m_rBoxLength(rhs.m_rBoxLength), m_zBoxLength(rhs.m_zBoxLength), m_r0Exp(rhs.m_r0Exp),
                    m_rBoxLeft(rhs.m_rBoxLeft), m_zBoxMid(rhs.m_zBoxMid), m_R0(rhs.m_R0), m_z0(rhs.m_z0),
                    m_psiAxis(rhs.m_psiAxis), m_psiEdge(rhs.m_psiEdge), m_Btor(rhs.m_Btor), m_Ip(rhs.m_Ip),
                    m_R(new double[rhs.m_NR]), m_z(new double[rhs.m_Nz]), m_psi(new double[rhs.m_NR*rhs.m_Nz]) {
                    std::copy(rhs.m_R, rhs.m_R + m_NR, m_R);
                    std::copy(rhs.m_z, rhs.m_z + m_Nz, m_z);
                    std::copy(rhs.m_psi, rhs.m_psi + m_NR*m_Nz, m_psi);
                }
                virtual ~equilibrium() {
                    delete [] m_R;
                    delete [] m_z;
                    delete [] m_psi;
                }

                double get_psi(const double R, const double z) const {
                    uint32_t i0;
                    uint32_t j0;
                    double R0, R1;
                    double z0, z1;
                    if( (R < *(m_R)) || (z > *(m_R + m_NR - 1)) || (z < *(m_z)) || (z > *(m_z + m_Nz -1))) {
                        return 0.0;
                    }
                    bool found = false;
                    for(uint32_t i = 0; i < m_NR-2; ++i) {
                        if((*(m_R + i) <= R) && (*(m_R + i + 1) >= R)) {
                            i0 = i;
                            R0 = *(m_R + i);
                            R1 = *(m_R + i + 1);
                            found = true;
                        }
                    }
                    if(!found) return 0.0;
                    found = false;
                    for(uint32_t j = 0; j < m_Nz-2; ++j) {
                        if((*(m_z + j) <= z) && (*(m_z + j + 1) >= z)) {
                            j0 = j;
                            z0 = *(m_z + j);
                            z1 = *(m_z + j + 1);
                            found = true;
                        }
                    }
                    if(!found) return 0.0;
                    double fQ00 = *(m_psi + i0 + j0*m_NR);
                    double fQ10 = *(m_psi + i0+1 + j0*m_NR);
                    double fQ11 = *(m_psi + i0+1 + (j0+1)*m_NR);
                    double fQ01 = *(m_psi + i0 + (j0+1)*m_NR);
                    double fR0 = ((R1-R)/(R1-R0) * fQ00) + ((R-R0)/(R1-R0) * fQ10);
                    double fR1 = ((R1-R)/(R1-R0) * fQ01) + ((R-R0)/(R1-R0) * fQ11);
                    return (z1-z)/(z1-z0) * fR0 + (z-z0)/(z1-z0) * fR1;
                }

                double get_rho(const double R, const double z) const {
                    return sqrt((m_psiAxis-get_psi(R,z))/(m_psiAxis-m_psiEdge));
                }

                std::string get_comment() const { return m_comment; }
                boost::python::tuple get_shape() const { return boost::python::make_tuple(m_NR, m_Nz); }

                std::vector<double> get_R() const {
                    return std::vector<double>(m_R, m_R + m_NR);
                }

                boost::python::list get_R_python() const {
                    boost::python::list output;
                    for(double * iter = m_R; iter != m_R + m_NR; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }

                std::vector<double> get_z() const {
                    return std::vector<double>(m_z, m_z + m_Nz);
                }

                boost::python::list get_z_python() const {
                    boost::python::list output;
                    for(double * iter = m_z; iter != m_z + m_Nz; ++iter) {
                        output.append(*iter);
                    }
                    return output;
                }
                double get_R0() const { return m_R0; }
                double get_z0() const { return m_z0; }
                double get_psiAxis() const { return m_psiAxis; }
                double get_psiEdge() const { return m_psiEdge; }
                double get_Btor() const { return m_Btor; }
                double get_Ip() const { return m_Ip; }
                double get_Rmin() const { return m_rBoxLeft; }
                double get_Rmax() const { return m_rBoxLeft + m_rBoxLength; }
                double get_zmin() const { return m_zBoxMid - m_zBoxLength/2.0; }
                double get_zmax() const { return m_zBoxMid + m_zBoxLength/2.0; }


            protected:
                std::string m_comment;
                uint32_t m_NR;
                uint32_t m_Nz;
                double m_rBoxLength;
                double m_zBoxLength;
                double m_r0Exp;
                double m_rBoxLeft;
                double m_zBoxMid;
                double m_R0;
                double m_z0;
                double m_psiAxis;
                double m_psiEdge;
                double m_Btor;
                double m_Ip;
                double * m_R;
                double * m_z;
                double * m_psi;
        };        
    }
}


#endif 
