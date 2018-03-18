/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_radios_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_RADIOS_ACTUATOR_H
#define CI_RADIOS_ACTUATOR_H

namespace argos {
   class CCI_RadiosActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {

   class CCI_RadiosActuator : virtual public CCI_Actuator {

   public:

      struct SInterface {
         SInterface(const std::string& str_id,
                    const std::vector<CByteArray>& vec_data = {}) :
            Id(str_id),
            Data(vec_data) {}
         std::string Id;
         std::vector<CByteArray> Data;
         using TVector = std::vector<SInterface>;
      };

   public:

      /**
       * Constructor
       */
      CCI_RadiosActuator() {}
      /**
       * Destructor.
       */
      virtual ~CCI_RadiosActuator() {}

   public:

      /**
       * Returns a reference to the radio interfaces.
       * @return A reference to the radio interfaces.
       */        
      SInterface::TVector& GetInterfaces();

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;
         
   };
}

#endif
