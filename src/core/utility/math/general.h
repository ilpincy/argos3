/**
 * @file <argos3/core/utility/math/general.h>
 *
 * @brief Contains the definition of some basic math functions.
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef GENERAL_H
#define GENERAL_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <vector>
#include <utility>
#include <cmath>

namespace argos {
    /****************************************/
    /****************************************/

    /**
     * Returns the absolute value of the passed argument.
     * @return the absolute value of the passed argument.
     */
    template<typename T> T Abs(const T& t_v) {
        if (t_v > T(0)) return t_v;
        if (t_v < T(0)) return -t_v;
        return T(0);
    }

    /**
     * Returns the absolute value of the passed argument.
     * Specific implementation for the SInt32 type.
     * @return the absolute value of the passed argument.
     * @see Abs()
     */
    inline SInt32 Abs(SInt32 t_v) {
        if (t_v > 0) return t_v;
        if (t_v < 0) return -t_v;
        return 0;
    }

    /**
     * Returns the absolute value of the passed argument.
     * Specific implementation for the Real type.
     * @return the absolute value of the passed argument.
     */
    inline Real Abs(Real t_v) {
        if (t_v > 0.0f) return t_v;
        if (t_v < 0.0f) return -t_v;
        return 0.0f;
    }

    /****************************************/
    /****************************************/

#ifdef ARGOS_USE_DOUBLE
#define Log  ::log
#define Sqrt ::sqrt
#define Exp ::exp
#define Mod ::fmod
#else
#define Log ::logf
#define Sqrt ::sqrtf
#define Exp ::expf
#define Mod ::fmodf
#endif

    /****************************************/
    /****************************************/

    /**
     * Returns the smaller of the two passed arguments.
     * This version accepts <tt>const</tt> references as arguments.
     * @return the smaller of the two passed arguments.
     */
    template<typename T> T Min(const T& t_v1, const T& t_v2) {
        return t_v1 < t_v2 ? t_v1 : t_v2;
    }

    /**
     * Returns the smaller of the two passed arguments.
     * This version accepts non-<tt>const</tt> references as arguments.
     * @return the smaller of the two passed arguments.
     */
    template<typename T> T& Min(T& t_v1, T& t_v2) {
        return t_v1 < t_v2 ? t_v1 : t_v2;
    }

    /**
     * Returns the bigger of the two passed arguments.
     * This version accepts <tt>const</tt> references as arguments.
     * @return the bigger of the two passed arguments.
     */
    template<typename T> T Max(const T& t_v1, const T& t_v2) {
        return t_v1 > t_v2 ? t_v1 : t_v2;
    }

    /**
     * Returns the bigger of the two passed arguments.
     * This version accepts non-<tt>const</tt> references as arguments.
     * @return the bigger of the two passed arguments.
     */
    template<typename T> T& Max(T& t_v1, T& t_v2) {
        return t_v1 > t_v2 ? t_v1 : t_v2;
    }

    /****************************************/
    /****************************************/

    /**
     * Returns the sign of the value of the passed argument.
     * @return -1 if the value is negative, +1 if it is positive, or 0 if the value is 0.
     */
    template<typename T> SInt32 Sign(const T& t_v) {
        if (t_v > T(0)) return 1;
        if (t_v < T(0)) return -1;
        return 0;
    }

    /****************************************/
    /****************************************/

    /**
     * Returns the square of the value of the passed argument.
     * @return the square of the value of the passed argument.
     */
    template<typename T> T Square(const T& t_v) {
        return t_v * t_v;
    }

    /****************************************/
    /****************************************/

    /**
     * Rounds the passed floating-point value to the closest lower integer.
     * Profiled, it takes 50% the time of <tt>floor()</tt>.
     * @return the closest lower integer to the passed value.
     */
    inline SInt32 Floor(Real f_value) {
        SInt32 nI = static_cast<SInt32> (f_value);
        if (f_value >= 0.0f) return nI;
        return nI - 1;
    }

    /**
     * Rounds the passed floating-point value to the closest higher integer.
     * Profiled, it takes 50% the time of <tt>ceil()</tt>.
     * @return the closest higher integer to the passed value.
     */
    inline SInt32 Ceil(Real f_value) {
        SInt32 nI = static_cast<SInt32> (f_value);
        if (nI < f_value) return nI + 1;
        return nI;
    }

    /**
     * Rounds the passed floating-point value to the closest integer.
     * If the passed value is >0, Floor() is called. Otherwise, Ceil() is called.
     * @return the closest integer.
     */
    inline SInt32 Round(Real f_value) {
        if (f_value > 0.0f) return Floor(f_value + 0.5f);
        return Ceil(f_value - 0.5f);
    }

    /**
     * Rounds the passed floating-point value to the integer closest to zero.
     * If the passed value is >0, Floor() is called. Otherwise, Ceil() is called.
     * @return the integer closest to zero.
     */
    inline SInt32 RoundClosestToZero(Real f_value) {
        if (f_value > 0.0f) return Floor(f_value);
        return Ceil(f_value);
    }

    /****************************************/
    /****************************************/

    /**
     * Tests whether a floating-point value is lower than another.
     * This function perform the following test:
     * <pre>
     *           Abs(f_value1 - f_value2)
     * --------------------------------------------- <= f_epsilon
     * Max (1.0f, Max(Abs(f_value1), Abs(f_value2)))
     * </pre>
     * @param f_value1 the first value to test
     * @param f_value2 the second value to test
     * @param f_epsilon the tolerance of the test
     * @return <tt>true</tt> if <tt>f_value1</tt> &lt; <tt>f_value2</tt>, false otherwise.
     * @see DoubleEq()
     */
    inline bool DoubleEqAbsolute(Real f_value1, Real f_value2, Real f_epsilon) {
        return Abs<Real > (f_value1 - f_value2) <= f_epsilon * Max<Real > (1.0f, Max<Real > (Abs<Real > (f_value1), Abs<Real > (f_value2)));
    }

    /**
     * Tests whether a floating-point value is lower than another.
     * This method is like DoubleEqAbsolute(), but with <tt>f_epsilon = 0.0001</tt>.
     * @param f_value1 the first value to test
     * @param f_value2 the second value to test
     * @return <tt>true</tt> if <tt>f_value1</tt> &lt; <tt>f_value2</tt>, false otherwise.    
     * @see DoubleEqAbsolute()
     */
    inline bool DoubleEq(Real f_value1, Real f_value2) {
        return Abs<Real > (f_value1 - f_value2) <= 0.0001f * Max<Real > (1.0f, Max<Real > (Abs<Real > (f_value1), Abs<Real > (f_value2)));
    }

    /****************************************/
    /****************************************/

    /**
     * Return the value of the linear interpolation
     * @param f_x the value of x that you want to interpolate
     * @param c_points vector of points NOTE: the vector has to be ordered in INCREASING ORDER.
     * @return value of y.
     */
    inline Real Interpolate(Real f_x, const std::vector<std::pair<Real, Real> >& c_points) {
        std::pair<Real, Real> cP0 = c_points.at(0);
        std::pair<Real, Real> cP1;
        for (UInt32 i = 1; i < c_points.size(); ++i) {
            cP1 = c_points.at(i);
            if (cP1.first >= f_x) {
                break;
            } else if (i < c_points.size() - 1) {
                cP0 = cP1;
            }
        }
        return (f_x * (cP0.second - cP1.second)) / (cP0.first - cP1.first)+(-cP1.first * cP0.second + cP0.first * cP1.second) / (cP0.first - cP1.first);;
    }
}

#endif
