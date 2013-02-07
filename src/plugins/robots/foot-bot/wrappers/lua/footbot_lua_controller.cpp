
/**
 * @file <argos3/plugins/robots/foot-bot/wrappers/lua/footbot_lua_controller.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "footbot_lua_controller.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheels_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotLuaController::CFootBotLuaController() :
      m_pcWheels(NULL),
      m_pcLEDs(NULL),
      m_pcProximity(NULL) {
   }

   /****************************************/
   /****************************************/

   void CFootBotLuaController::CreateLuaVariables() {
      /*
       * Create a table that will contain the state of the robot
       */
      lua_newtable(GetLuaState());
      /*
       * Go through devices and add the necessary items to the table
       */
      if(HasActuator("footbot_wheels")) {
         m_pcWheels = new CLuaDifferentialSteeringActuator(
            *GetActuator<CCI_FootBotWheelsActuator>("footbot_wheels"),
            GetLuaState(),
            "wheel_speed");
         m_pcWheels->CreateLuaVariables();
      }
      if(HasActuator("footbot_leds")) {
         m_pcLEDs = new CLuaLEDsActuator(
            *GetActuator<CCI_FootBotLEDsActuator>("footbot_leds"),
            GetLuaState(),
            "leds");
         m_pcLEDs->CreateLuaVariables();
      }
      if(HasSensor("footbot_proximity")) {
         m_pcProximity = new CLuaRingProximitySensor(
            *GetSensor<CCI_FootBotProximitySensor>("footbot_proximity"),
            GetLuaState(),
            "proximity");
         m_pcProximity->CreateLuaVariables();
      }
      /*
       * Set the name of the table
       */
      lua_setglobal(GetLuaState(), "footbot");
   }

   /****************************************/
   /****************************************/

   void CFootBotLuaController::SensorReadingsToLuaVariables() {
      /*
       * Put the footbot state table on top
       */
      lua_getglobal(GetLuaState(), "footbot");
      /*
       * Go through the sensors
       */
      if(m_pcProximity) {
         m_pcProximity->ReadingsToLuaVariables();
      }
      /*
       * Pop the footbot state table
       */
      lua_pop(GetLuaState(), 1);
   }

   /****************************************/
   /****************************************/

   void CFootBotLuaController::LuaVariablesToActuatorSettings() {
      /*
       * Put the footbot state table on top
       */
      lua_getglobal(GetLuaState(), "footbot");
      /*
       * Go through the actuators
       */
      if(m_pcWheels) m_pcWheels->LuaVariablesToSettings();
      if(m_pcLEDs) m_pcLEDs->LuaVariablesToSettings();
      /*
       * Pop the footbot state table
       */
      lua_pop(GetLuaState(), 1);
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(CFootBotLuaController, "footbot_lua_controller");

}
