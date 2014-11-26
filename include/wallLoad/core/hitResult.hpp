#ifndef include_wallLoad_core_hitResult_hpp
#define include_wallLoad_core_hitResult_hpp

#include <boost/python.hpp>
#include <wallLoad/core/vektor.hpp>

namespace wallLoad {
    namespace core {
        /*! \brief Class to store the hit result of an intersection check.
         *
         * This class stores the intersection point, the information if a hit occurred and what element got hit.
         */
        class hitResult
        {
            public:
                /*! \brief Default constructor
                 *
                 * This constructor initializes an empty hit result.
                 */
                hitResult() : hasHit(false), hitPoint(), element(-1) {}
                /*! \brief Constructor
                 *
                 * This constructor initializes the hit result with hit information and hit point.
                 */
                hitResult(const bool HasHit, const vektor & HitPoint) :
                    hasHit(HasHit), hitPoint(HitPoint), element(-1) {
                }
                /*! \brief Copy constructor */
                hitResult(const hitResult & rhs) :
                    hasHit(rhs.hasHit), hitPoint(rhs.hitPoint),
                    element(rhs.element) {
                }
                /*! \brief Destructor */
                virtual ~hitResult() {}

                /*! \brief Assignment operator
                 *
                 * This operator copies the information of the given hit result to the current instance.
                 */
                hitResult & operator= (const hitResult & rhs) {
                    if(this != &rhs) {
                        hasHit = rhs.hasHit;
                        hitPoint = rhs.hitPoint;
                        element = rhs.element;
                    }
                    return *this;
                }
                /*! \brief Bool operator
                 *
                 * This operator evaluates the instance as bool.
                 * True if the hit occurred, false if not.
                 */
                explicit operator bool() const { return hasHit; }
                /*! \brief Calculate the distance between the hit point and the given point.
                 *
                 * This function calculates the distance between the hit point and the given point.
                 */
                double get_distance(const vektor & rhs) const {
                    return (hitPoint - rhs).get_length();
                }
            public:
                bool hasHit; /*!< \brief Information if the hit occurred. */
                vektor hitPoint; /*!< \brief Position where the intersection occurs. */
                int32_t element; /*!< \brief Number of the element that got hit. */ 
        };
    }
}


#endif 
