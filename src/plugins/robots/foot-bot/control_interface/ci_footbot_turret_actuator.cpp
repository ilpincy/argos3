/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_turret_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

  /****************************************/
  /****************************************/

  const CRange<SInt32> CCI_FootBotTurretActuator::SPEED_RANGE(-4,4);
  const CRange<Real>   CCI_FootBotTurretActuator::NORMALIZED_SPEED_RANGE(-1.0,1.0);

  /****************************************/
  /****************************************/

#ifdef ARGOS_WITH_LUA
  int LuaTurretSetRotation(lua_State* pt_lua_state) {
    /* Check parameters */
    if(lua_gettop(pt_lua_state) != 1) {
      return luaL_error(pt_lua_state, "robot.turret.set_rotation() expects 1 argument");
    }
    luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
    /* Perform action */
    CLuaUtility::GetDeviceInstance<CCI_FootBotTurretActuator>(pt_lua_state, "turret")->
      SetRotation(CRadians(lua_tonumber(pt_lua_state, 1)));
    return 0;
  }
#endif

#ifdef ARGOS_WITH_LUA
  int LuaTurretSetRotationSpeed(lua_State* pt_lua_state) {
    /* Check parameters */
    if(lua_gettop(pt_lua_state) != 1) {
      return luaL_error(pt_lua_state, "robot.turret.set_rotation_speed() expects 1 argument");
    }
    luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
    /* Perform action */
    CLuaUtility::GetDeviceInstance<CCI_FootBotTurretActuator>(pt_lua_state, "turret")->
      SetRotationSpeed(lua_tonumber(pt_lua_state, 1));
    return 0;
  }
#endif

#ifdef ARGOS_WITH_LUA
  int LuaTurretSetSpeedControlMode(lua_State* pt_lua_state) {
    /* Check parameters */
    if(lua_gettop(pt_lua_state) != 0) {
      return luaL_error(pt_lua_state, "robot.turret.set_speed_control_mode() expects no arguments");
    }
    /* Perform action */
    CLuaUtility::GetDeviceInstance<CCI_FootBotTurretActuator>(pt_lua_state, "turret")->
      SetSpeedControlMode();
    return 0;
  }
#endif

#ifdef ARGOS_WITH_LUA
  int LuaTurretSetPositionControlMode(lua_State* pt_lua_state) {
    /* Check parameters */
    if(lua_gettop(pt_lua_state) != 0) {
      return luaL_error(pt_lua_state, "robot.turret.set_position_control_mode() expects no arguments");
    }
    /* Perform action */
    CLuaUtility::GetDeviceInstance<CCI_FootBotTurretActuator>(pt_lua_state, "turret")->
      SetPositionControlMode();
    return 0;
  }
#endif

#ifdef ARGOS_WITH_LUA
  int LuaTurretSetPassiveMode(lua_State* pt_lua_state) {
    /* Check parameters */
    if(lua_gettop(pt_lua_state) != 0) {
      return luaL_error(pt_lua_state, "robot.turret.set_passive_mode() expects no arguments");
    }
    /* Perform action */
    CLuaUtility::GetDeviceInstance<CCI_FootBotTurretActuator>(pt_lua_state, "turret")->
      SetPassiveMode();
    return 0;
  }
#endif

  /****************************************/
  /****************************************/

  void CCI_FootBotTurretActuator::SetActiveWithRotation(const CRadians& c_angle) {
    SetPositionControlMode();
    SetRotation(c_angle);
  }

  /****************************************/
  /****************************************/

  void CCI_FootBotTurretActuator::SetSpeedControlMode() {
    SetMode(MODE_SPEED_CONTROL);
  }

  /****************************************/
  /****************************************/

  void CCI_FootBotTurretActuator::SetPositionControlMode() {
    SetMode(MODE_POSITION_CONTROL);
  }

  /****************************************/
  /****************************************/

  void CCI_FootBotTurretActuator::SetPassiveMode() {
    SetMode(MODE_PASSIVE);
  }

  /****************************************/
  /****************************************/

#ifdef ARGOS_WITH_LUA
  void CCI_FootBotTurretActuator::CreateLuaState(lua_State* pt_lua_state) {
    CLuaUtility::OpenRobotStateTable(pt_lua_state, "turret");
    CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
    CLuaUtility::AddToTable(pt_lua_state, "set_rotation", &LuaTurretSetRotation);
    CLuaUtility::AddToTable(pt_lua_state, "set_rotation_speed", &LuaTurretSetRotationSpeed);
    CLuaUtility::AddToTable(pt_lua_state, "set_position_control_mode", &LuaTurretSetPositionControlMode);
    CLuaUtility::AddToTable(pt_lua_state, "set_speed_control_mode", &LuaTurretSetSpeedControlMode);
    CLuaUtility::AddToTable(pt_lua_state, "set_passive_mode", &LuaTurretSetPassiveMode);
    CLuaUtility::CloseRobotStateTable(pt_lua_state);
  }
#endif

  /****************************************/
  /****************************************/

}
