/**
 * @file <argos3/plugins/robots/generic/simulator/radios_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "radios_default_actuator.h"
#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRadiosDefaultActuator::CRadiosDefaultActuator() :
      m_pcRadioEquippedEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get and enable omndirectional radio equipped entity */
         m_pcRadioEquippedEntity = &(c_entity.GetComponent<CRadioEquippedEntity>("radios"));
         /* Create a configuration settings for each radio in the container */
         m_vecInterfaces.reserve(m_pcRadioEquippedEntity->GetInstances().size());
         /* Populate the descriptors */
         for(CRadioEquippedEntity::SInstance& s_instance : m_pcRadioEquippedEntity->GetInstances()) {
            m_vecInterfaces.emplace_back(s_instance.Radio.GetId());
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the radios default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_RadiosActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the radios default actuator", ex);
      }
   }
   
   /****************************************/
   /****************************************/

   void CRadiosDefaultActuator::Update() {
      for(size_t i = 0; i < m_vecInterfaces.size(); ++i) {
         CRadioEntity& cRadio = m_pcRadioEquippedEntity->GetRadio(i);
         /* Create operation instance */
         CSendOperation cSendOperation(cRadio, m_vecInterfaces[i].Messages);
         /* Calculate the coarse range of the transmitting radio */
         CVector3 cRange(1.0f, 1.0f, 1.0f);
         cRange *= cRadio.GetRange();
         /* Get positional index */
         CPositionalIndex<CRadioEntity>* pcRadioIndex =
            &(cRadio.GetMedium().GetIndex());
         /* Transmit the messages to receiving radios in the space */
         pcRadioIndex->ForEntitiesInBoxRange(cRadio.GetPosition(), cRange, cSendOperation);
         /* Flush messages from the control interface */
         m_vecInterfaces[i].Messages.clear();
      }
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultActuator::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* Clear any messages in the interface */
         s_interface.Messages.clear();
      }
   }

   /****************************************/
   /****************************************/

   bool CRadiosDefaultActuator::CSendOperation::operator()(CRadioEntity& c_recv_radio) {
      if(&c_recv_radio != &m_cRadio) {
         Real fDistance = (c_recv_radio.GetPosition() - m_cRadio.GetPosition()).Length();
         if(fDistance < m_cRadio.GetRange()) {
            for(const CByteArray& c_message : m_cMessages) {
               c_recv_radio.ReceiveMessage(m_cRadio.GetPosition(), c_message);
            }
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CRadiosDefaultActuator,
                     "radios", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",

                     "A generic radio actuator to send messages to nearby radios.",
                     "This radio actuator implementation allows an arbitary number of messages\n"
                     "containing an arbitary number of bytes to be sent to nearby radios. The\n" 
                     "implementation of this actuator is very basic and any concepts such as\n"
                     "throughput, addressing, or formatting of a message's contents is beyond the\n"
                     "scope of this actuator's implementation.\n\n"

                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <radios implementation=\"default\" medium=\"radios\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"

                     "The 'medium' attribute sets the id of the radio medium declared in the <media>\n"
                     "XML section.\n\n"

                     "OPTIONAL XML CONFIGURATION\n\n"

                     "None.\n",

                     "Usable"
   );

   /****************************************/
   /****************************************/

}
