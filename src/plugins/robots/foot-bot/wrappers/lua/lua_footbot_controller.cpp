
/**
 * @file <argos3/plugins/robots/foot-bot/wrappers/lua/lua_footbot_controller.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_footbot_controller.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/generic/control_interface/ci_ring_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>

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
      if(HasActuator("differential_steering")) {
         m_pcWheels = new CLuaDifferentialSteeringActuator(
            *GetActuator<CCI_DifferentialSteeringActuator>("differential_steering"),
            GetLuaState(),
            "wheel_speed");
         m_pcWheels->CreateLuaVariables();
      }
      if(HasActuator("leds")) {
         m_pcLEDs = new CLuaLEDsActuator(
            *GetActuator<CCI_LEDsActuator>("leds"),
            GetLuaState(),
            "leds");
         m_pcLEDs->CreateLuaVariables();
      }
      if(HasSensor("ring_proximity")) {
         m_pcProximity = new CLuaRingProximitySensor(
            *GetSensor<CCI_RingProximitySensor>("ring_proximity"),
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

   REGISTER_CONTROLLER(CFootBotLuaController, "lua_footbot_controller");

}
