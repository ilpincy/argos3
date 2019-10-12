/**
 * @file <argos3/plugins/robots/generic/simulator/tags_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "tags_default_actuator.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTagsDefaultActuator::CTagsDefaultActuator() :
      m_pcTagEquippedEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CTagsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcTagEquippedEntity = 
         &(c_entity.GetComponent<CTagEquippedEntity>("tags"));
      m_tSettings.resize(m_pcTagEquippedEntity->GetInstances().size());
   }

   /****************************************/
   /****************************************/

   void CTagsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_TagsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the tags default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTagsDefaultActuator::Update() {
      m_pcTagEquippedEntity->SetTagPayloads(m_tSettings);
   }

   /****************************************/
   /****************************************/

   void CTagsDefaultActuator::Reset() {
      SetAllPayloads("");
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CTagsDefaultActuator,
                     "tags", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The tags actuator.",

                     "This actuator controls the payloads of a group of tags. The idea"
                     "is to represent an LCD screen that displays different tags. For a"
                     "complete description of its usage, refer to the ci_tags_actuator.h"
                     " file.\n\n"

                     "REQUIRED XML CONFIGURATION\n\n"

                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <tags implementation=\"default\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"

                     "OPTIONAL XML CONFIGURATION\n\n"

                     "None.\n",

                     "Usable"
   );

   /****************************************/
   /****************************************/

}
