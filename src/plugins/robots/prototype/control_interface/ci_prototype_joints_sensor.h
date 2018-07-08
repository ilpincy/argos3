/**
 * @file <argos3/plugins/robots/prototype/control_interface/ci_prototype_joints_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_PROTOTYPE_JOINTS_SENSOR_H
#define CCI_PROTOTYPE_JOINTS_SENSOR_H

namespace argos {
   class CCI_PrototypeJointsSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_PrototypeJointsSensor : virtual public CCI_Sensor {

   public:

      struct SSensor {
         using TVector = std::vector<SSensor*>;
         SSensor(const std::string& str_id) :
            Id(str_id) {}
         std::string Id;
         Real Value;
      };

   public:

      /**
       * Destructor.
       */
      virtual ~CCI_PrototypeJointsSensor() {}

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected: 
      SSensor::TVector m_vecSensors;
   };
}

#endif
