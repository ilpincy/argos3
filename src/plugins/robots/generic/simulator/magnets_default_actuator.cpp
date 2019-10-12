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
                     "The (electro)magnet actuator.",

                     "This actuator is used to modify the magnetic field of a magnet entity in the\n"
                     "simulation. A magnetic entity has a passive and an active field. The overall\n"
                     "field of a magnetic entity is calculated as:\n\n"
                     "  field = passive_field + (current x active_field)\n\n"
                     "This actuator allows you to set the current, a scalar multiplier of the active\n"
                     "field.\n\n"

                     "REQUIRED XML CONFIGURATION\n\n"

                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <magnets implementation=\"default\"/>\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"

                     "OPTIONAL XML CONFIGURATION\n\n"

                     "None.\n",

                     "Under development"
   );

   /****************************************/
   /****************************************/

}
