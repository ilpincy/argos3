/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_radios_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_RADIOS_SENSOR_H
#define CI_RADIOS_SENSOR_H

namespace argos {
	class CCI_RadiosSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {
   
   class CCI_RadiosSensor: virtual public CCI_Sensor {
      
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
      CCI_RadiosSensor() {}
      
      /**
       * Destructor
       */
      virtual ~CCI_RadiosSensor() {}
   
   public:

      /**
       * Returns a const reference to the radio interfaces.
       * @return A const reference to the radio interfaces.
       */
      const SInterface::TVector& GetInterfaces() const;
      
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:
 
      SInterface::TVector m_vecInterfaces;
      
   };
   
}

#endif
