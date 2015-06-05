/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_distance_scanner_default_actuator.h"

namespace argos {

	 const Real CFootBotDistanceScannerDefaultActuator::RPM_TO_RADIANS_PER_SEC = ARGOS_PI / 30.0f;

   /****************************************/
   /****************************************/

   CFootBotDistanceScannerDefaultActuator::CFootBotDistanceScannerDefaultActuator() :
      m_fDesiredRotationSpeed(0.0f),
      m_unDesiredMode(CFootBotDistanceScannerEquippedEntity::MODE_OFF) {}

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDistanceScannerEquippedEntity = &(c_entity.GetComponent<CFootBotDistanceScannerEquippedEntity>("distance_scanner"));
      m_pcDistanceScannerEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::SetAngle(const CRadians& c_angle) {
      m_unDesiredMode = CFootBotDistanceScannerEquippedEntity::MODE_POSITION_CONTROL;
      m_cDesiredRotation = c_angle;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::SetRPM(Real f_rpm) {
      m_unDesiredMode = CFootBotDistanceScannerEquippedEntity::MODE_SPEED_CONTROL;
      m_fDesiredRotationSpeed = f_rpm * RPM_TO_RADIANS_PER_SEC;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::Enable() {
      m_unDesiredMode = CFootBotDistanceScannerEquippedEntity::MODE_POSITION_CONTROL;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::Disable() {
      m_unDesiredMode = CFootBotDistanceScannerEquippedEntity::MODE_OFF;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::Update() {
      m_pcDistanceScannerEquippedEntity->SetMode(m_unDesiredMode);
      if(m_unDesiredMode == CFootBotDistanceScannerEquippedEntity::MODE_POSITION_CONTROL) {
         m_pcDistanceScannerEquippedEntity->SetRotation(m_cDesiredRotation);
      }
      else if(m_unDesiredMode == CFootBotDistanceScannerEquippedEntity::MODE_SPEED_CONTROL) {
         m_pcDistanceScannerEquippedEntity->SetRotationSpeed(m_fDesiredRotationSpeed);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerDefaultActuator::Reset() {
      m_cDesiredRotation = CRadians::ZERO;
      m_fDesiredRotationSpeed = 0.0f;
      m_unDesiredMode = CFootBotDistanceScannerEquippedEntity::MODE_OFF;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CFootBotDistanceScannerDefaultActuator,
                     "footbot_distance_scanner", "default",
                     "Carlo Pinciroli [ilpincy@gmail.com]",
                     "1.0",
                     "The foot-bot distance scanner actuator.",
                     "This actuator controls the foot-bot distance scanner. For a complete\n"
                     "description of its usage, refer to the ci_footbot_distance_scanner_actuator\n"
                     "file.\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <footbot_distance_scanner implementation=\"default\" />\n"
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
