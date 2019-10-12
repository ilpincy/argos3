/**
 * @file <argos3/plugins/simulator/actuators/quadrotor_speed_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "quadrotor_speed_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQuadRotorSpeedDefaultActuator::CQuadRotorSpeedDefaultActuator() :
      m_pcQuadRotorEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get the quadrotor component */
         m_pcQuadRotorEntity = &(c_entity.GetComponent<CQuadRotorEntity>("quadrotor"));
         /* Check whether the control methods is unset - only one is allowed */
         if(m_pcQuadRotorEntity->GetControlMethod() == CQuadRotorEntity::NO_CONTROL) {
            /* Get the robot body */
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            /* Set the speed control method */
            m_pcQuadRotorEntity->SetControlMethod(CQuadRotorEntity::SPEED_CONTROL);
         }
         else {
            THROW_ARGOSEXCEPTION("Can't associate a quadrotor speed actuator to entity \"" << c_entity.GetId() << "\" because it conflicts with a previously associated quadrotor actuator.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting quadrotor speed actuator to entity \"" << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_QuadRotorSpeedActuator::Init(t_tree);
         Reset();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in quadrotor speed actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::SetLinearVelocity(const CVector3& c_velocity) {
      m_sDesiredSpeedData.Velocity = c_velocity;
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::SetRotationalSpeed(const CRadians& c_speed) {
      m_sDesiredSpeedData.RotSpeed = c_speed;      
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::Update() {
      m_pcQuadRotorEntity->SetSpeedControlData(m_sDesiredSpeedData);
   }

   /****************************************/
   /****************************************/

   void CQuadRotorSpeedDefaultActuator::Reset() {
      m_sDesiredSpeedData.Velocity = CVector3();
      m_sDesiredSpeedData.RotSpeed = CRadians::ZERO;
      Update();
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CQuadRotorSpeedDefaultActuator,
                  "quadrotor_speed", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The quadrotor speed actuator.",

                  "This actuator controls the speed of a quadrotor robot. For a\n"
                  "complete description of its usage, refer to the\n"
                  "ci_quadrotor_speed_actuator.h file.\n\n"

                  "REQUIRED XML CONFIGURATION\n\n"

                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <quadrotor_speed implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"

                  "OPTIONAL XML CONFIGURATION\n\n"

                  "None.\n\n"
                  ,
                  "Usable"
   );
