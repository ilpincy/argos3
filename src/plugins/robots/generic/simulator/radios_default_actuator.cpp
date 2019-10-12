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
         CTxOperation cTxOperation(cRadio, m_vecInterfaces[i].Data);
         /* Calculate the range of the transmitting radio */
         CVector3 cTxRange(1.0f,1.0f,1.0f);
         cTxRange *= (cRadio.GetRange() * 0.5f);
         /* Get positional index */
         CPositionalIndex<CRadioEntity>* pcRadioIndex =
            &(cRadio.GetMedium().GetIndex());
         /* Transmit the data to receiving radios in the space */
         pcRadioIndex->ForEntitiesInBoxRange(cRadio.GetPosition(), cTxRange, cTxOperation);
         /* Flush data from the control interface */
         m_vecInterfaces[i].Data.clear();
      }
   }

   /****************************************/
   /****************************************/

   void CRadiosDefaultActuator::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* Clear any data in the interface */
         s_interface.Data.clear();
      }
   }

   /****************************************/
   /****************************************/

   CRadiosDefaultActuator::CTxOperation::CTxOperation(const CRadioEntity& c_tx_radio,
                                                      const std::vector<CByteArray>& c_tx_data) :
      m_cTxRadio(c_tx_radio),
      m_cTxData(c_tx_data) {}

   /****************************************/
   /****************************************/

   bool CRadiosDefaultActuator::CTxOperation::operator()(CRadioEntity& c_rx_radio) {
      if(&c_rx_radio != &m_cTxRadio) {
         const CVector3& cRxRadioPosition = c_rx_radio.GetPosition();
         const CVector3& cTxRadioPosition = m_cTxRadio.GetPosition();
         Real fDistance = (cRxRadioPosition - cTxRadioPosition).Length();
         if(fDistance < m_cTxRadio.GetRange()) {
            for(const CByteArray& c_data : m_cTxData) {
               c_rx_radio.ReceiveData(cTxRadioPosition, c_data);
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
