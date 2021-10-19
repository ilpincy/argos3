/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_RANGEFINDERS_SENSOR_H
#define CCI_BUILDERBOT_RANGEFINDERS_SENSOR_H

namespace argos {
   class CCI_BuilderBotRangefindersSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <map>
#include <tuple>

namespace argos {

   class CCI_BuilderBotRangefindersSensor : public CCI_Sensor {

   public:

      struct SInterface {
         SInterface(const std::string& str_label) :
            Label(str_label),
            Proximity(0.05),
            Illuminance(0.0),
            Anchor(std::get<std::string>(m_mapSensorConfig.at(str_label))),
            PositionOffset(std::get<CVector3>(m_mapSensorConfig.at(str_label))),
            OrientationOffset(std::get<CQuaternion>(m_mapSensorConfig.at(str_label))) {}
         std::string Label;
         Real Proximity;
         Real Illuminance;
         const std::string& Anchor;
         const CVector3& PositionOffset;
         const CQuaternion& OrientationOffset;
         using TVector = std::vector<SInterface*>;
      };

      virtual ~CCI_BuilderBotRangefindersSensor() {}

      const SInterface::TVector& GetInterfaces() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;

      static const std::map<std::string, std::tuple<std::string, CVector3, CQuaternion> > m_mapSensorConfig;

   };

}

#endif
