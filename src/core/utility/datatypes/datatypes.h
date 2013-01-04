/**
 * @file <argos3/core/utility/datatypes/datatypes.h>
 *
 * This file provides the basic data type definitions of ARGoS.
 * The definitions provided here are cross-platform. For instance, a argos::SInt16
 * variable will be a 16-bit signed integer on all platforms ARGoS is ported to.
 *
 * The argos::Real type wraps the native C++ types <tt>float</tt> and <tt>double</tt>.
 * Whether argos::Real is defined as <tt>float</tt> or <tt>double</tt> depends on the
 * compilation flags set when building ARGoS in file src/core/config.h.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DATATYPES_H
#define DATATYPES_H

#include <argos3/core/config.h>

#ifdef __cplusplus
#include <iostream>
#endif

/**
 * Collects all ARGoS code.
 */
#ifdef __cplusplus
namespace argos {
#endif
    
   /**
    * The basic floating point type in ARGoS.
    * Real is defined either as <tt>float</tt> or <tt>double</tt>, depending on the flags set
    * for compilation.
    */
#ifdef ARGOS_USE_DOUBLE
   typedef double Real;
#else
   typedef float Real;
#endif

   /**
    * 8-bit signed integer.
    */
   typedef signed char SInt8;

#ifdef __cplusplus
   /**
    * Overloaded operator to have C++ streams handle correctly 8-bit signed integers.
    * @see SInt8
    */
   inline std::ostream& operator<<(std::ostream& c_os, const SInt8 n_value) {
      c_os << static_cast<signed int>(n_value);
      return c_os;
   }
#endif
   /**
    * 8-bit unsigned integer.
    */
   typedef unsigned char UInt8;
#ifdef __cplusplus
   /**
    * Overloaded operator to have C++ streams handle correctly 8-bit unsigned integers.
    * @see UInt8
    */
   inline std::ostream& operator<<(std::ostream& c_os, const UInt8 un_value) {
      c_os << static_cast<unsigned int>(un_value);
      return c_os;
   }
#endif
   /**
    * 16-bit signed integer.
    */
   typedef signed short SInt16;
   /**
    * 16-bit unsigned integer.
    */
   typedef unsigned short UInt16;

#ifdef __dsPIC30
   /**
    * 32-bit signed integer.
    */
   typedef signed long int SInt32;
   /**
    * 32-bit unsigned integer.
    */
   typedef unsigned long int UInt32;
#else
   /**
    * 32-bit signed integer.
    */
   typedef signed int SInt32;
   /**
    * 32-bit unsigned integer.
    */
   typedef unsigned int UInt32;
#endif
   
   /**
    * 64-bit signed integer.
    */
   typedef signed long long SInt64;
   /**
    * 64-bit unsigned integer.
    */
   typedef unsigned long long UInt64;
   
   
#ifdef __cplusplus
}
#endif

#endif
