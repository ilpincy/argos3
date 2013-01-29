/**
 * @file <argos3/core/utility/configuration/argos_exception.h>
 *
 * @brief This file provides the definition of the main ARGoS exception.
 *
 * Apart from the exception class, this file also provides several utility macros that should always be used to throw
 * ARGoS exceptions. The simplest way to throw an ARGoS exception is to use the following:
 * <pre>
 * THROW_ARGOSEXCEPTION("The value you provided is wrong.")
 * </pre>
 * you can also used variable values in the message, as follows:
 * <pre>
 * THROW_ARGOSEXCEPTION("The value \"" << myVar << "\" is wrong.")
 * </pre>
 * It is also possible to nest exceptions, to provide more details to the user when an error occurs. See the following example:
 * <pre>
 * try {
 *    DoStuff(); // throws an ARGoS exception
 *    DoMoreStuff(); // this is not executed
 * }
 * catch(CARGoSException& ex) {
 *    THROW_ARGOSEXCEPTION_NESTED("There was an error while doing stuff", ex);
 * }
 * </pre>
 *
 * In addition, it is possible to define assertions for debug purposes. Assertions are conditions on variable values. They are
 * typically nice to have while developing to be sure that the code is behaving correctly (i.e., array bound checks), but in
 * later phases, when the code must be released, the cost of the check is considered an unnecessary burden. To support
 * debug assertions, ARGoS provides the ARGOS_ASSERT macro. Here is an example:
 * <pre>
 * void MyFunction(UInt32 un_index) {
 *    ARGOS_ASSERT(un_index < 10, "Index out of bounds.");
 *    ...
 * }
 * </pre>
 * When the code is compiled in debug, this macro checks the condition. If the conditions is false, an ARGoS exception is
 * thrown with the passed message. When the code is compiled in release, the ARGOS_ASSERT macro does not produce any code at all,
 * thus the resulting binary is much faster.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_EXCEPTION_H
#define ARGOS_EXCEPTION_H

namespace argos {
   class CARGoSException;
}

#include <stdexcept>
#include <sstream>

namespace argos {

   /**
    * The exception that wraps all errors in ARGoS.
    *
    * This class is used to wrap all errors occurring in ARGoS. You should never explicitly throw this exception
    * directly, but you should instead use the provided macros THROW_ARGOSEXCEPTION() and THROW_ARGOSEXCEPTION_NESTED().
    */
   class CARGoSException : public std::exception {

   public:

      /**
       * Class constructor.
       * @param str_what the error message that explains what happened and why the exception was thrown
       * @param pc_nested a pointer to the nested exception
       */
      CARGoSException(const std::string& str_what, std::exception* pc_nested = NULL) throw() :
         m_strWhat("[FATAL] " + str_what), m_pcNested(pc_nested) {
         if (m_pcNested != NULL) {
            std::ostringstream w;
            w << m_strWhat
              << std::endl
              << m_pcNested->what();
            m_strWhat = w.str();
         }
      }
      /**
       * Class destructor.
       */
      virtual ~CARGoSException() throw() {
      }

      /**
       * Returns the error message that explains what happened and why the exception was thrown
       * The returned message also contains messages of nested exceptions, if any.
       */
      virtual const char* what() const throw() {
         return m_strWhat.c_str();
      }

   private:

      /**
       * The error message that explains what happened and why the exception was thrown
       * The message also contains messages of nested exceptions, if any.
       */
      std::string m_strWhat;
      /** A pointer to the nested exception, or NULL of there is none */
      std::exception* m_pcNested;

   };

}

/**
 * This macro throws an ARGoS exception with the passed message.
 */
#define THROW_ARGOSEXCEPTION(message) { std::ostringstream what; what << message; throw CARGoSException(what.str()); }
/**
 * This macro throws an ARGoS exception with the passed message and nesting the passed exception.
 */
#define THROW_ARGOSEXCEPTION_NESTED(message, nested) { std::ostringstream what; what << message; throw CARGoSException(what.str(), &nested); }

#ifndef NDEBUG
/**
 * When code is compiled in debug, this macro throws an ARGoS exception with the passed message if the specified condition is not met.
 * When code is compiled in release, this macro does nothing.
 */
#define ARGOS_ASSERT(condition, message) { if ( !(condition) ) THROW_ARGOSEXCEPTION(message); }
#else
#define ARGOS_ASSERT(condition, message)
#endif

#endif
