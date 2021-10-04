/**
 * @file <argos3/plugins/robots/generic/simulator/radios_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "radios_default_sensor.h"
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRadiosDefaultSensor::CRadiosDefaultSensor() :
      m_pcRadioEquippedEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_bShowRays(false) {}

   /****************************************/
   /****************************************/

   void CRadiosDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get the radio equipped entity */
         m_pcRadioEquippedEntity = &(c_entity.GetComponent<CRadioEquippedEntity>("radios"));
         /* Resize the interface vector for each radio in the radio equipped entity */
         m_vecInterfaces.reserve(m_pcRadioEquippedEntity->GetInstances().size());
         /* For each radio, create an interface */
         for(CRadioEquippedEntity::SInstance& s_instance :
             m_pcRadioEquippedEntity->GetInstances()) {
            /* Clear the existing message */
            m_vecInterfaces.emplace_back(s_instance.Radio.GetId());
         }
         /* Get controllable entity */
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the radios default sensor", ex);
      }

      /* sensor is enabled by default */
      Enable();
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_RadiosSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the radios default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultSensor::Update() {
      /* sensor is disabled--nothing to do */
      if (IsDisabled()) {
        return;
      }
      for(size_t i = 0; i < m_pcRadioEquippedEntity->GetInstances().size(); ++i) {
         CRadioEntity& cRadio = m_pcRadioEquippedEntity->GetRadio(i);
         /* Clear messages in the interface */
         m_vecInterfaces[i].Messages.clear();
         /* Copy messages from the radio entity to the control interface */
         for(const std::pair<CVector3, CByteArray>& c_message : cRadio.GetMessages()) {
            m_vecInterfaces[i].Messages.emplace_back(c_message.second);
            if(m_bShowRays) {
               CRay3 cRay(c_message.first, cRadio.GetPosition());
               m_pcControllableEntity->GetCheckedRays().emplace_back(!c_message.second.Empty(), cRay);
            }
         }
         /* Clear messages in the radio entity */
         cRadio.GetMessages().clear();
      }
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultSensor::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* Clear the existing message */
         s_interface.Messages.clear();
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CRadiosDefaultSensor,
                   "radios", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",

                   "A generic radio sensor to receive messages from nearby radios.",
                   "This radio sensor implementation allows an arbitary number of messages\n"
                   "containing an arbitary number of bytes to be received from nearby robots. The\n"
                   "implementation is very basic and any concepts such as throughput, addressing,\n"
                   "or formatting of a message's contents is beyond the scope of this sensor's\n"
                   "implementation\n\n"

                   "This sensor is enabled by default.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <radios implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "None.\n",

                   "Usable"
   );

}
