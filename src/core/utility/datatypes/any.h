/**
 * @file <argos3/core/utility/datatypes/any.h>
 *
 * @brief This file provides the definition of the any type.
 *
 * The any type is a generic container that allows one to store objects of different
 * types in a standard container. The implementation in this file is heavily
 * inspired by the corresponding boost::any implementation
 * (http://www.boost.org/libs/any)
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef ANY_H
#define ANY_H

#include <argos3/core/utility/configuration/argos_exception.h>

#include <cxxabi.h>
#include <typeinfo>
#include <algorithm>

namespace argos {

   class CAny {

   public:

      /**
       * Constructor
       */
      CAny() :
         m_pcRef(NULL) {}

      /**
       * Constructor with templetized argument
       */
      template<typename T>
      CAny(const T& t_object) :
         m_pcRef(new CReference<T>(t_object)) {}

      /**
       * Copy constructor
       * Clone the reference to the object, if the reference is not NULL
       */
      CAny(const CAny& c_any) :
         m_pcRef(c_any.m_pcRef ? c_any.m_pcRef->Clone() : NULL) {}

      /**
       * Destructor
       */
      ~CAny() {
         if(m_pcRef != NULL)
            delete m_pcRef;
      }

      /**
       * Redefinition of the swap function of the std library
       */
      CAny& swap(CAny& c_any) {
         std::swap(m_pcRef, c_any.m_pcRef);
         return *this;
      }

      /**
       * Assignment operator when object is operand
       */
      template <typename T>
      CAny& operator=(const T& t_object) {
         CAny(t_object).swap(*this);
         return *this;
      }

      /**
       * Assignment operator when any type is operand
       */
      CAny& operator=(const CAny& c_any) {
         CAny(c_any).swap(*this);
         return *this;
      }

   public:

      /**
       * Abstract reference to object
       */
      class CAbstractReference {
      public:
         /**
          * Destructor
          */
         virtual ~CAbstractReference() {}
         /**
          * Returns the type info on the referenced object
          */
         virtual const std::type_info& GetType() const = 0;
         /**
          * Clone the reference
          */
         virtual CAbstractReference* Clone() const = 0;
      };

      /**
       * Actual templetized reference to object
       */
      template<typename T>
      class CReference : public CAbstractReference {
      public:
         CReference(const T& t_object) : m_tObject(t_object) {}
         /**
          * Destructor
          */
         virtual ~CReference() {}
         /**
          * @return The type info on the referenced object
          */
         virtual const std::type_info& GetType() const {
            return typeid(T);
         }
         /**
          * Clone the reference
          */
         virtual CAbstractReference* Clone() const {
            return new CReference<T>(m_tObject);
         }
      public:

         /**
          * Actual referenced object
          */
         T m_tObject;
      };
      
   public:

      /**
       * Pointer to object reference
       */
      CAbstractReference* m_pcRef;

   };

   /**
    * Performs a cast on the any type to the desired type, when the any type is
    * passed by non-const pointer
    */
   template<typename T>
   T* any_cast(CAny* pc_any) {
      /* NULL pointer passed? */
      if(pc_any == NULL)
         return NULL;
      /* OK, Non-NULL pointer; do types match? */
      if(pc_any->m_pcRef->GetType() == typeid(T))
         /* Types match -> return cast object */
         return &(static_cast<CAny::CReference<T>*>(pc_any->m_pcRef))->m_tObject;
      else
         /* Types don't match -> return NULL */
         return NULL;
   }

   /**
    * Performs a cast on the any type to the desired type, when the any type is
    * passed by const pointer
    */
   template<typename T>
   const T* any_cast(const CAny* pc_any) {
      /* NULL pointer passed? */
      if(pc_any == NULL)
         return NULL;
      /* OK, Non-NULL pointer; do types match? */
      if(pc_any->m_pcRef->GetType() == typeid(T))
         /* Types match -> return cast object */
         return &(static_cast<CAny::CReference<T>*>(pc_any->m_pcRef))->m_tObject;
      else
         /* Types don't match -> return NULL */
         return NULL;
   }

   /**
    * Performs a cast on the any type to the desired type, when the any type is
    * passed by const reference
    */
   template<typename T>
   const T& any_cast(const CAny& c_any) {
      /* Fall back to using pass-by-pointer version */
      const T* ptResult = any_cast<T>(&c_any);
      /* Did the cast succeed? */
      if(ptResult != NULL)
         return *ptResult;
      else {
         char* pchDemangledOperandType = abi::__cxa_demangle(c_any.m_pcRef->GetType().name(), NULL, NULL, NULL);
         char* pchDemangledTargetType = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
         THROW_ARGOSEXCEPTION("Failed any_cast conversion from " <<
                              pchDemangledOperandType <<
                              " to " <<
                              pchDemangledTargetType);
      }
   }

   /**
    * Performs a cast on the any type to the desired type, when the any type is
    * passed by non-const reference
    */
   template<typename T>
   T& any_cast(CAny& c_any) {
      /* Fall back to using pass-by-pointer version */
      T* ptResult = any_cast<T>(&c_any);
      /* Did the cast succeed? */
      if(ptResult != NULL)
         return *ptResult;
      else {
         char* pchDemangledOperandType = abi::__cxa_demangle(c_any.m_pcRef->GetType().name(), NULL, NULL, NULL);
         char* pchDemangledTargetType = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
         THROW_ARGOSEXCEPTION("Failed any_cast conversion from " <<
                              pchDemangledOperandType <<
                              " to " <<
                              pchDemangledTargetType);
      }
   }

}

#endif
