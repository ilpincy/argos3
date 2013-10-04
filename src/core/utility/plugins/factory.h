/**
 * @file <argos3/core/utility/plugins/factory.h>
 *
 * @brief This file defines the basic factory template.

 * <p>This template is very versatile and allows you to create your own factory
 * with just a couple of lines of code.
 *
 * <p>Let us say that you have a class hierarchy such as:
 *
 * <pre>
 *   class MyClass { ... };
 *   class MyFirstDerivate : public MyClass { ... };
 *   class MySecondDerivate : public MyClass { ... };
 * </pre>
 *
 * and you want to register <tt>MyFirstDerivate</tt> and
 * <tt>MySecondDerivate</tt>. In this case, it is enough to add to after the
 * definition of <tt>class MyClass</tt>:
 *
 * <pre>
 *   typedef CFactory<MyClass> TFactoryMyClass;
 *   #define REGISTER_MYCLASS(CLASSNAME, LABEL) REGISTER_SYMBOL(MyClass, CLASSNAME, LABEL)
 * </pre>
 *
 * Then, in your .cpp files that implement <tt>MyFirstDerivate</tt> and
 * <tt>MySecondDerivate</tt>, add:
 *
 * <pre>
 *   REGISTER_MYCLASS(MyFirstDerivate, "my_first_derivate")
 *   REGISTER_MYCLASS(MySecondDerivate, "my_second_derivate")
 * </pre>
 *
 * <p>Now, if somewhere in your code you want to create an object of type
 * <tt>MyFirstDerivate</tt>, just write:
 *
 * <pre>
 *   MyFirstDerivate* x = TFactoryMyClass::New("my_first_derivate");
 * </pre>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FACTORY_H
#define FACTORY_H

#include <argos3/core/utility/configuration/argos_exception.h>
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>

namespace argos {

   /**
    * Basic factory template
    */
   template<typename TYPE>
   class CFactory {

   public:
      /**
       * Pointer to the function that creates objects of type <tt>TYPE</tt>
       */
      typedef TYPE* TCreator();

      /**
       * A struct containing the information about the registered types
       */
      struct STypeInfo {
         TCreator* Creator;
         std::string Author;
         std::string Version;
         std::string BriefDescription;
         std::string LongDescription;
         std::string Status;
      };

      /**
       * The map of registered <tt>TYPE</tt>s
       */
      typedef std::map<std::string, STypeInfo*> TTypeMap;

   public:
      /**
       * Creates and returns the <tt>TYPE</tt> map
       * @return The <tt>TYPE</tt> map
       */
      static TTypeMap& GetTypeMap();

      /**
       * Registers a new <tt>TYPE</tt> creator in the factory
       * @param str_label The label associated to the <tt>TYPE</tt> creator
       * @param str_author The author of the plugin
       * @param str_version The version of the plugin
       * @param str_brief_desc The brief description of the plugin
       * @param str_long_desc The long description of the plugin
       * @param str_status The status of the plugin
       * @param pc_creator The <tt>TYPE</tt> creator of the factory
       */
      static void Register(const std::string& str_label,
                           const std::string& str_author,
                           const std::string& str_version,
                           const std::string& str_brief_desc,
                           const std::string& str_long_desc,
                           const std::string& str_status,
                           TCreator* pc_creator);
      /**
       * Creates a new object of type <tt>TYPE</tt>
       * @param str_label The label of the <tt>TYPE</tt> to create
       * @return A new object of type <tt>TYPE</tt>
       */
      static TYPE* New(const std::string& str_label);

      /**
       * Returns <tt>true</tt> if the given label exists in the <tt>TYPE</tt> map
       * @return <tt>true</tt> if the given label exists in the <tt>TYPE</tt> map
       */
      static bool Exists(const std::string& str_label);

      /**
       * Prints the list of registered labels
       * @param c_os The <tt>std::ostream</tt> to write into
       */
      static void Print(std::ostream& c_os);

      /**
       * Frees up all used memory.
       */
      static void Destroy();
   };

/*
 * Include the actual template implementation
 */
#include <argos3/core/utility/plugins/factory_impl.h>

/**
 * Macro to register a symbol into the factory
 * @param BASECLASS The base class of the hiearchy of types to register (corresponds to <tt>TYPE</tt> in the factory)
 * @param CLASSNAME The actual class to register
 * @param LABEL The label to associate to <tt>CLASSNAME</tt>
 * @param AUTHOR The author of the class <tt>CLASSNAME</tt>
 * @param VERSION Version information for the code in <tt>CLASSNAME</tt>
 * @param BRIEF_DESCRIPTION A brief description of what <tt>CLASSNAME</tt> is
 * @param LONG_DESCRIPTION A detailed description of what <tt>CLASSNAME</tt> is and does
 * @param STATUS Status information (usable, draft, todos, etc) for the code in <tt>CLASSNAME</tt>
 */
#define REGISTER_SYMBOL(BASECLASS,                                      \
                        CLASSNAME,                                      \
                        LABEL,                                          \
                        AUTHOR,                                         \
                        VERSION,                                        \
                        BRIEF_DESCRIPTION,                              \
                        LONG_DESCRIPTION,                               \
                        STATUS)                                         \
   namespace argos {                                                    \
      extern "C" {                                                      \
         BASECLASS* BASECLASS ## CLASSNAME ## Creator() {               \
            return new CLASSNAME;                                       \
         }                                                              \
      }                                                                 \
      class C ## BASECLASS ## CLASSNAME ## Proxy {                      \
      public:                                                           \
      C ## BASECLASS ## CLASSNAME ## Proxy() {                          \
         CFactory<BASECLASS>::                                          \
            Register(LABEL,                                             \
                     AUTHOR,                                            \
                     VERSION,                                           \
                     BRIEF_DESCRIPTION,                                 \
                     LONG_DESCRIPTION,                                  \
                     STATUS,                                            \
                     BASECLASS ## CLASSNAME ## Creator);                \
      }                                                                 \
      };                                                                \
      C ## BASECLASS ## CLASSNAME ## Proxy BASECLASS ## CLASSNAME ## _p; \
   }

}

#endif
