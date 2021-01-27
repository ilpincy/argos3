/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_encoder_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_turret_encoder_default_sensor.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotTurretEncoderDefaultSensor::CFootBotTurretEncoderDefaultSensor() :
      m_pcTurretEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CFootBotTurretEncoderDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcTurretEntity = &(c_entity.GetComponent<CFootBotTurretEntity>("turret"));

      /* sensor is enabled by default */
      Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretEncoderDefaultSensor::Update() {
      /* sensor is disabled--nothing to do */
      if (IsDisabled()) {
        return;
      }
      m_cRotation = m_pcTurretEntity->GetRotation();
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretEncoderDefaultSensor::Enable() {
     m_pcTurretEntity->Enable();
     CCI_Sensor::Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretEncoderDefaultSensor::Disable() {
     m_pcTurretEntity->Disable();
     CCI_Sensor::Disable();
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretEncoderDefaultSensor::Reset() {
      m_cRotation = CRadians::ZERO;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotTurretEncoderDefaultSensor,
                   "footbot_turret_encoder", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot turret encoder sensor.",
                   "This sensor accesses the foot-bot turret encoder. For a complete\n"
                   "description of its usage, refer to the ci_footbot_turret_encoder_sensor\n"
                   "file.\n\n"

                   "This sensor is enabled by default.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_turret_encoder implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.\n",
                   "Usable"
      );

}
