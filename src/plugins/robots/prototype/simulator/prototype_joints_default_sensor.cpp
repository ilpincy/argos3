/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joints_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_joints_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeJointsDefaultSensor::CPrototypeJointsDefaultSensor() :
      m_pcJointEquippedEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcJointEquippedEntity = &(c_entity.GetComponent<CPrototypeJointEquippedEntity>("joints"));
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultSensor::Init(TConfigurationNode& t_tree) {
     TConfigurationNodeIterator itJoint("joint");
     for(itJoint = itJoint.begin(&t_tree);
         itJoint != itJoint.end();
         ++itJoint) {
         /* parse the joint id */
         std::string strJointId;
         GetNodeAttribute(*itJoint, "id", strJointId);
         /* get joint */
         CPrototypeJointEntity& cJoint = m_pcJointEquippedEntity->GetJoint(strJointId);
         /* get sensor */
         CPrototypeJointEntity::SSensor& sInstance = cJoint.GetSensor();
         /* configure sensor */
         std::string strMode;
         GetNodeAttribute(*itJoint, "mode", strMode);
         if(strMode == "disabled") {
            sInstance.Mode = CPrototypeJointEntity::SSensor::EMode::DISABLED;
         }
         else if(strMode == "position") {
            sInstance.Mode = CPrototypeJointEntity::SSensor::EMode::POSITION;
         }
         else if(strMode == "velocity") {
            sInstance.Mode = CPrototypeJointEntity::SSensor::EMode::VELOCITY;
         }
         else {
            THROW_ARGOSEXCEPTION("specified sensor mode for " << strJointId << " is not implemented");
         }
         /* create the sensor's interface */
         m_vecSimulatedSensors.emplace_back(strJointId, sInstance);
      }
      for(SSimulatedSensor& s_sensor : m_vecSimulatedSensors) {
         /* add joint actuators to the base class */
         m_vecSensors.push_back(&s_sensor);
      }
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultSensor::Update() {
      for(SSimulatedSensor& s_sensor : m_vecSimulatedSensors) {
         s_sensor.Value = s_sensor.Instance.Value;
      }
   }

   /****************************************/
   /****************************************/

   void CPrototypeJointsDefaultSensor::Reset() {
      for(SSimulatedSensor& s_sensor : m_vecSimulatedSensors) {
         s_sensor.Value = 0;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPrototypeJointsDefaultSensor,
                   "joints", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The prototype joints sensor: monitors a prototype entity's joints.",
                   "This sensor is used to monitor the joints inside a prototype entity. To monitor\n"
                   "a joint, add a joint child node to the joints node. Each child node has two\n"
                   "required attributes.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <joints implementation=\"default\">\n"
                   "          <joint id=\"joint0\" mode=\"velocity\" />\n"
                   "          <joint id=\"joint1\" mode=\"position\" />\n"
                   "          ...\n"
                   "        </joints>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "The 'id' attribute specifies which joint in the prototype joint equipped entity\n"
                   "we are interested in monitoring. The 'mode' attribute has three options:\n"
                   "disabled, position, and velocity.  The disabled mode is self-explanatory. The\n"
                   "position mode measures the displacement of the joint from its initial position\n"
                   "or orientation (depending on whether the specified joint is prismatic or\n"
                   "revolute). The reading from the joint is reported in either meters or radians\n"
                   "respectively. The velocity mode measures the how much the position or\n"
                   "orientation of the joint is changing every second.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None.",
                   "Usable"
      );

   /****************************************/
   /****************************************/

}
