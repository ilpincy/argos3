/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_torque_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_turret_torque_sensor.h"

namespace argos {
   
   /****************************************/
    /****************************************/
   
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::RAW_FORCEXY_RANGE(-1200.0f, 1200.0f);
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::NORMALIZED_FORCEXY_RANGE(-1.0f, 1.0f);
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::RAW_POLAR_RANGE(0.0f, 1200.0f);
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::NORMALIZED_POLAR_RANGE(0.0f, 1.0f);
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::RAW_FORCE_TORQUE_RANGE(-1200.0f, 1200.0f);
   const CRange<Real>     CCI_FootBotTurretTorqueSensor::NORMALIZED_TORQUE_RANGE(-1.0f, 1.0f);
   const CRange<CRadians> CCI_FootBotTurretTorqueSensor::ANGULAR_RANGE(CRadians(-ARGOS_PI), CRadians(ARGOS_PI));

   /****************************************/
   /****************************************/
   
}
