/**
 * @file <argos3/plugins/robots/generic/simulator/magnets_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "magnets_default_actuator.h"
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CMagnetsDefaultActuator::CMagnetsDefaultActuator() :
      m_pcMagnetEquippedEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   void CMagnetsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         /* Get the magnet equipped entity */
         m_pcMagnetEquippedEntity = &(c_entity.GetComponent<CMagnetEquippedEntity>("magnets"));
         /* Allocate memory for a control interface for each instance */
         m_vecInterfaces.reserve(m_pcMagnetEquippedEntity->GetInstances().size());
         /* Populate the interfaces */
         for(CMagnetEquippedEntity::SInstance& s_instance : m_pcMagnetEquippedEntity->GetInstances()) {
            m_vecInterfaces.emplace_back(s_instance.Magnet.GetId());
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the magnets default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CMagnetsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_MagnetsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the magnets default actuator", ex);
      }
   }

   
   /****************************************/
   /****************************************/

   void CMagnetsDefaultActuator::Update() {
      for(size_t i = 0; i < m_vecInterfaces.size(); ++i) {
         CMagnetEntity& cMagnet = m_pcMagnetEquippedEntity->GetMagnet(i);
         /* Update each magnet field by setting the magnet's current */
         cMagnet.SetCurrent(m_vecInterfaces[i].Current);
      }
   }

   /****************************************/
   /****************************************/

   void CMagnetsDefaultActuator::Reset() {
      for(SInterface& s_interface : m_vecInterfaces) {
         /* Set the current to zero */
         s_interface.Current = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CMagnetsDefaultActuator,
                     "magnets", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "TODO SHORT",
                     "TODO LONG",
                     "Under development"
   );
}
