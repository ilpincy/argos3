/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joints_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_joints_default_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeJointsDefaultActuator::CPrototypeJointsDefaultActuator() :
      m_pcJointEquippedEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcJointEquippedEntity = &(c_entity.GetComponent<CPrototypeJointEquippedEntity>("joints"));
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultActuator::Init(TConfigurationNode& t_tree) {
     TConfigurationNodeIterator itJoint("joint");
     for(itJoint = itJoint.begin(&t_tree);
         itJoint != itJoint.end();
         ++itJoint) {
         /* parse the joint id */
         std::string strJointId;
         GetNodeAttribute(*itJoint, "id", strJointId);
         /* parse the maximum impulse */
         Real fMaxImpulse;
         GetNodeAttribute(*itJoint, "max_impulse", fMaxImpulse);
         /* get joint */
         CPrototypeJointEntity& cJoint = m_pcJointEquippedEntity->GetJoint(strJointId);
         if(cJoint.GetType() == CPrototypeJointEntity::EType::PRISMATIC ||
            cJoint.GetType() == CPrototypeJointEntity::EType::REVOLUTE) {
            /* get actuator */
            CPrototypeJointEntity::SActuator& sInstance = cJoint.GetActuator();
            /* configure actuator */
            std::string strMode;
            GetNodeAttribute(*itJoint, "mode", strMode);
            if(strMode == "disabled") {
               sInstance.Mode = CPrototypeJointEntity::SActuator::EMode::DISABLED;
            }
            else if(strMode == "position") {
               sInstance.Mode = CPrototypeJointEntity::SActuator::EMode::POSITION;
            }
            else if(strMode == "velocity") {
               sInstance.Mode = CPrototypeJointEntity::SActuator::EMode::VELOCITY;
            }
            else {
               THROW_ARGOSEXCEPTION("Actuator mode \"" << strMode << 
                                    "\" for joint \"" << strJointId << 
                                    "\" " << " is not implemented");
            }
            sInstance.MaxImpulse = fMaxImpulse;
            sInstance.Target = 0.0f;
            /* create the actuator's interface */
            m_vecSimulatedActuators.emplace_back(strJointId, 0.0, sInstance);
         }
         else {
            THROW_ARGOSEXCEPTION("Actuators can only be set on prismatic or revolute joints");
         }
      }
      for(SSimulatedActuator& s_actuator : m_vecSimulatedActuators) {
         /* add joint actuators to the base class */
         m_vecActuators.push_back(&s_actuator);
      }
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultActuator::Update() {
      for(SSimulatedActuator& s_actuator : m_vecSimulatedActuators) {
         s_actuator.Instance.Target = s_actuator.Target;
      }
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultActuator::Reset() {
      for(SSimulatedActuator& s_actuator : m_vecSimulatedActuators) {
         s_actuator.Target = 0.0;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CPrototypeJointsDefaultActuator,
                     "joints", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The prototype joints actuator: controls a prototype entity's joints.",
                     "This actuator is used to control the joints inside a prototype entity. To\n"
                     "control a joint, add a joint child node to the joints node. Each child node has\n"
                     "three attributes, all of which are required.\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <joints implementation=\"default\">\n"
                     "          <joint id=\"joint0\" mode=\"velocity\" max_impulse=\"0.002f\"/>\n"
                     "          <joint id=\"joint1\" mode=\"position\" max_impulse=\"0.005f\"/>\n"
                     "          ...\n"
                     "        </joints>\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"
                     "The 'id' attribute specifies which joint in the prototype joint equipped entity\n"
                     "we are interested in controlling. The 'mode' attribute has three options:\n"
                     "disabled, position, and velocity.  The disabled mode is self-explanatory. The\n"
                     "position and velocity modes enable closed loop position control and closed loop\n"
                     "velocity control. The 'max_impulse' attribute defines the maximum impulse of\n"
                     "the actuator in kg m/s for prismatic joints and kg m^2/s for revolute joints.\n\n"
                     "OPTIONAL XML CONFIGURATION\n\n"
                     "None.",
                     "Usable"
      );

   /****************************************/
   /****************************************/

}
