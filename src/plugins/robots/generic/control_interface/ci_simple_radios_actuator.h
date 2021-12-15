/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_simple_radios_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_SIMPLE_RADIOS_ACTUATOR_H
#define CI_SIMPLE_RADIOS_ACTUATOR_H

namespace argos {
   class CCI_SimpleRadiosActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {

   class CCI_SimpleRadiosActuator : virtual public CCI_Actuator {

   public:

      struct SInterface {
         SInterface(const std::string& str_id,
                    const std::vector<CByteArray>& vec_messages = {}) :
            Id(str_id),
            Messages(vec_messages) {}
         std::string Id;
         std::vector<CByteArray> Messages;
         using TVector = std::vector<SInterface>;
      };

   public:

      /**
       * Constructor
       */
      CCI_SimpleRadiosActuator() {}
      /**
       * Destructor.
       */
      virtual ~CCI_SimpleRadiosActuator() {}

   public:

      /**
       * Returns a reference to the radio interfaces.
       * @return A reference to the radio interfaces.
       */        
      SInterface::TVector& GetInterfaces() {
         return m_vecInterfaces;
      }

#ifdef ARGOS_WITH_LUA
      static int LuaSendMessage(lua_State* pt_lua_state);

      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;
         
   };
}

#endif
