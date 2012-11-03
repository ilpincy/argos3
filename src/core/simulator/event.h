/**
 * @file core/simulator/event.h
 */

#ifndef OPERATION_H
#define OPERATION_H

#include <argos3/core/utility/configuration/argos_exception.h>

#include <map>
#include <cxxabi.h>
#include <typeinfo>

namespace argos {

   template <typename T>
   class CEventHandler {

   public:

      virtual ~CEventHandler() {}

      inline const std::string& GetId() const {
         return m_strId;
      }

      inline void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

      virtual void Execute(T& t_operand) = 0;

   private:

      std::string m_strId;

   };

   template <typename T>
   class CEvent {

   public:

      virtual ~CEvent() {}
      void AddHandler(CEventHandler<T>& c_operator) {
         m_tHandlers[c_operator.GetId()] = &c_operator;
      }

      void CallHandlers(T& t_data) {
         for(typename THandlers::iterator it = m_tHandlers.begin();
             it != m_tHandlers.end();
             ++it) {
            it->second->Execute(t_data);
         }
      }

      void CallHandler(const std::string& str_id,
                       T& t_data) {
         typename THandlers::iterator it = m_tHandlers.find(str_id);
         if(it != m_tHandlers.end()) {
            it->second->Execute(t_data);
         }
         else {
            char* pchDemangledType = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
            THROW_ARGOSEXCEPTION("Operand of type " <<
                                 pchDemangledType <<
                                 " does not have an operator with id = \"" <<
                                 str_id <<
                                 "\"");
         }
      }

   private:

      typedef std::map<std::string, CEventHandler<T>*> THandlers;
      THandlers m_tHandlers;

   };

}

#endif
