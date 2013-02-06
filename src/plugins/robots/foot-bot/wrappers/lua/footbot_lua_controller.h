#include <argos3/core/wrappers/lua/lua_controller.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheels_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>

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

      CCI_FootBotWheelsActuator* m_pcWheels;
      CCI_FootBotLEDsActuator* m_pcLEDs;
      CCI_FootBotProximitySensor* m_pcProximity;

   };

}
