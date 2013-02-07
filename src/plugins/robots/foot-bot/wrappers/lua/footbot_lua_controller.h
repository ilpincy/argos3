
/**
 * @file <argos3/plugins/robots/foot-bot/wrappers/lua/footbot_lua_controller.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include <argos3/core/wrappers/lua/lua_controller.h>
#include <argos3/plugins/robots/generic/wrappers/lua/lua_ring_proximity_sensor.h>
#include <argos3/plugins/robots/generic/wrappers/lua/lua_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/wrappers/lua/lua_leds_actuator.h>

namespace argos {

   class CFootBotLuaController : public CLuaController {

   public:

      CFootBotLuaController();

      virtual ~CFootBotLuaController() {}

   protected:

      virtual void CreateLuaVariables();

      virtual void SensorReadingsToLuaVariables();

      virtual void LuaVariablesToActuatorSettings();

   private:

      CLuaDifferentialSteeringActuator* m_pcWheels;
      CLuaLEDsActuator*                 m_pcLEDs;
      CLuaRingProximitySensor*          m_pcProximity;

   };

}
