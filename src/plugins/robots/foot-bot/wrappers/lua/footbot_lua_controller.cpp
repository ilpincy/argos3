#include "footbot_lua_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

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
         m_pcWheels = GetActuator<CCI_FootBotWheelsActuator>("footbot_wheels");
         lua_pushliteral(GetLuaState(), "wheel_speed");
         lua_newtable   (GetLuaState()               );
         lua_pushliteral(GetLuaState(),        "left");
         lua_pushnumber (GetLuaState(),             0);
         lua_settable   (GetLuaState(),            -3);
         lua_pushliteral(GetLuaState(),       "right");
         lua_pushnumber (GetLuaState(),             0);
         lua_settable   (GetLuaState(),            -3);
         lua_settable   (GetLuaState(),            -3);
      }
      if(HasActuator("footbot_leds")) {
         m_pcLEDs = GetActuator<CCI_FootBotLEDsActuator>("footbot_leds");
         lua_pushliteral(GetLuaState(), "leds");
         lua_newtable   (GetLuaState());
         for(size_t i = 0; i < CCI_FootBotLEDsActuator::NUM_LEDS; ++i) {
            lua_pushnumber(GetLuaState(), i+1);
            lua_newtable(GetLuaState());
            lua_pushliteral(GetLuaState(), "red");
            lua_pushnumber(GetLuaState(), 0);
            lua_settable(GetLuaState(), -3);
            lua_pushliteral(GetLuaState(), "green");
            lua_pushnumber(GetLuaState(), 0);
            lua_settable(GetLuaState(), -3);
            lua_pushliteral(GetLuaState(), "blue");
            lua_pushnumber(GetLuaState(), 0);
            lua_settable(GetLuaState(), -3);
            lua_settable(GetLuaState(), -3);
         }
         lua_settable(GetLuaState(), -3);
      }
      if(HasSensor("footbot_proximity")) {
         m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
         lua_pushliteral(GetLuaState(), "proximity");
         lua_newtable   (GetLuaState());
         for(size_t i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i) {
            lua_pushnumber(GetLuaState(), i+1);
            lua_newtable(GetLuaState());
            lua_pushliteral(GetLuaState(), "angle");
            lua_pushnumber(GetLuaState(), m_pcProximity->GetReadings()[i].Angle.GetValue());
            lua_settable(GetLuaState(), -3);
            lua_pushliteral(GetLuaState(), "value");
            lua_pushnumber(GetLuaState(), m_pcProximity->GetReadings()[i].Value);
            lua_settable(GetLuaState(), -3);
            lua_settable(GetLuaState(), -3);
         }
         lua_settable(GetLuaState(), -3);
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
         lua_getfield(GetLuaState(), -1, "proximity");
         for(size_t i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i) {
            lua_pushnumber(GetLuaState(), i+1);
            lua_gettable  (GetLuaState(), -2);
            lua_pushnumber(GetLuaState(), m_pcProximity->GetReadings()[i].Value);
            lua_setfield  (GetLuaState(), -2, "value");
            lua_pop(GetLuaState(), 1);
         }
         lua_pop(GetLuaState(), 1);
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
      if(m_pcWheels) {
         lua_getfield(GetLuaState(), -1, "wheel_speed");
         lua_getfield(GetLuaState(), -1, "left");
         lua_getfield(GetLuaState(), -2, "right");
         m_pcWheels->SetLinearVelocity(lua_tonumber(GetLuaState(), -2),
                                       lua_tonumber(GetLuaState(), -1));
         lua_pop(GetLuaState(), 3);
      }
      if(m_pcLEDs) {
         lua_getfield(GetLuaState(), -1, "leds");
         CCI_FootBotLEDsActuator::TSettings tLEDs(CCI_FootBotLEDsActuator::NUM_LEDS);
         for(size_t i = 0; i < CCI_FootBotLEDsActuator::NUM_LEDS; ++i) {
            lua_pushnumber(GetLuaState(), i+1);
            lua_gettable  (GetLuaState(), -2);
            lua_getfield  (GetLuaState(), -1, "red");
            lua_getfield  (GetLuaState(), -2, "green");
            lua_getfield  (GetLuaState(), -3, "blue");
            tLEDs[i].Set(lua_tonumber(GetLuaState(), -3),
                         lua_tonumber(GetLuaState(), -2),
                         lua_tonumber(GetLuaState(), -1));
            LOGERR << "[DEBUG] tLEDs[" << i << "] = " << tLEDs[i] << std::endl;
            lua_pop(GetLuaState(), 4);
         }
         lua_pop(GetLuaState(), 1);
         m_pcLEDs->SetAllColors(tLEDs);
      }
      /*
       * Pop the footbot state table
       */
      lua_pop(GetLuaState(), 1);
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(CFootBotLuaController, "footbot_lua_controller");

}
