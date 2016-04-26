/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_turret_default_actuator.h"

namespace argos {

	 const Real RPM_TO_RADIANS_PER_SEC = ARGOS_PI / 30.0f;

   /****************************************/
   /****************************************/

   CFootBotTurretDefaultActuator::CFootBotTurretDefaultActuator() :
      m_pcTurretEntity(NULL),
      m_unDesiredMode(CFootBotTurretEntity::MODE_OFF) {}

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcTurretEntity = &(c_entity.GetComponent<CFootBotTurretEntity>("turret"));
      m_pcTurretEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::SetRotation(const CRadians& c_angle) {
      m_pcTurretEntity->SetDesiredRotation(c_angle);
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::SetRotationSpeed(SInt32 n_speed_pulses) {
      m_pcTurretEntity->SetDesiredRotationSpeed(RPM_TO_RADIANS_PER_SEC * n_speed_pulses);
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::SetMode(ETurretModes e_mode) {
      m_unDesiredMode = e_mode;
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::Update() {
      m_pcTurretEntity->SetMode(m_unDesiredMode);
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretDefaultActuator::Reset() {
      m_unDesiredMode = CFootBotTurretEntity::MODE_OFF;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CFootBotTurretDefaultActuator,
                     "footbot_turret", "default",
                     "Carlo Pinciroli [ilpincy@gmail.com]",
                     "1.0",
                     "The foot-bot turret actuator.",
                     "This actuator controls the foot-bot turret. For a complete\n"
                     "description of its usage, refer to the ci_footbot_turret_actuator\n"
                     "file.\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <footbot_turret implementation=\"default\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"
                     "OPTIONAL XML CONFIGURATION\n\n"
                     "None for the time being.\n",
                     "Usable"
      );

}
