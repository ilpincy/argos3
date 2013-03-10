/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_gripper_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/
   
   CRadians CCI_FootBotGripperActuator::LOCKED_NEGATIVE(-ARGOS_PI * 0.5f);
   CRadians CCI_FootBotGripperActuator::LOCKED_POSITIVE(ARGOS_PI * 0.5f);
   CRadians CCI_FootBotGripperActuator::UNLOCKED(0.0f);

   CRange<CRadians> APERTURE_RANGE(CCI_FootBotGripperActuator::LOCKED_NEGATIVE,
                                   CCI_FootBotGripperActuator::LOCKED_POSITIVE);
   
   /****************************************/
   /****************************************/

   CCI_FootBotGripperActuator::CCI_FootBotGripperActuator() :
      m_cAperture(UNLOCKED) {}

   /****************************************/
   /****************************************/

   void CCI_FootBotGripperActuator::SetAperture(const CRadians& c_aperture) {
      m_cAperture = c_aperture;
      m_cAperture.SignedNormalize();
      APERTURE_RANGE.TruncValue(m_cAperture);
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotGripperActuator::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "gripper");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "aperture");
      lua_pushnumber(pt_lua_state, m_cAperture.GetValue());
      lua_settable  (pt_lua_state, -3);
      lua_settable  (pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotGripperActuator::LuaVariablesToSettings(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "gripper");
      lua_getfield(pt_lua_state, -1, "aperture");
      SetAperture(CRadians(lua_tonumber(pt_lua_state, -1)));
      lua_pop(pt_lua_state, 2);
   }
#endif

   /****************************************/
   /****************************************/

}
