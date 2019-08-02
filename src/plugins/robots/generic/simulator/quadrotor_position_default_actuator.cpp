/**
 * @file <argos3/plugins/simulator/actuators/quadrotor_position_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "quadrotor_position_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQuadRotorPositionDefaultActuator::CQuadRotorPositionDefaultActuator() :
      m_pcQuadRotorEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get the quadrotor component */
         m_pcQuadRotorEntity = &(c_entity.GetComponent<CQuadRotorEntity>("quadrotor"));
         /* Check whether the control methods is unset - only one is allowed */
         if(m_pcQuadRotorEntity->GetControlMethod() == CQuadRotorEntity::NO_CONTROL) {
            /* Get the robot body */
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            /* Set the position control method */
            m_pcQuadRotorEntity->SetControlMethod(CQuadRotorEntity::POSITION_CONTROL);
         }
         else {
            THROW_ARGOSEXCEPTION("Can't associate a quadrotor position actuator to entity \"" << c_entity.GetId() << "\" because it conflicts with a previously associated quadrotor actuator.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting quadrotor position actuator to entity \"" << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_QuadRotorPositionActuator::Init(t_tree);
         Reset();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in quadrotor position actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::SetAbsolutePosition(const CVector3& c_pos) {
      m_sDesiredPosData.Position = c_pos;
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::SetRelativePosition(const CVector3& c_pos) {
      m_sDesiredPosData.Position = c_pos;
      m_sDesiredPosData.Position.Rotate(m_pcEmbodiedEntity->GetOriginAnchor().Orientation);
      m_sDesiredPosData.Position += m_pcEmbodiedEntity->GetOriginAnchor().Position;
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::SetAbsoluteYaw(const CRadians& c_yaw) {
      m_sDesiredPosData.Yaw = c_yaw;
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::SetRelativeYaw(const CRadians& c_yaw) {
      CRadians cYAngle, cXAngle;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(m_sDesiredPosData.Yaw, cYAngle, cXAngle);
      m_sDesiredPosData.Yaw += c_yaw;
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::Update() {
      m_pcQuadRotorEntity->SetPositionControlData(m_sDesiredPosData);
   }

   /****************************************/
   /****************************************/

   void CQuadRotorPositionDefaultActuator::Reset() {
      m_sDesiredPosData.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      CRadians cYAngle, cXAngle;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(m_sDesiredPosData.Yaw, cYAngle, cXAngle);
      Update();
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CQuadRotorPositionDefaultActuator,
                  "quadrotor_position", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The quadrotor position actuator.",

                  "This actuator controls the position of a quadrotor robot. For a\n"
                  "complete description of its usage, refer to the\n"
                  "ci_quadrotor_position_actuator.h file.\n\n"

                  "REQUIRED XML CONFIGURATION\n\n"

                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <quadrotor_position implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"

                  "OPTIONAL XML CONFIGURATION\n\n"

                  "None for the time being.\n\n"
                  ,
                  "Usable"
   );
