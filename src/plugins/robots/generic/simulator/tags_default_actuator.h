/**
 * @file <argos3/plugins/robots/generic/simulator/tags_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef TAGS_DEFAULT_ACTUATOR_H
#define TAGS_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CTagsDefaultActuator;
   class CTagMedium;
}

#include <argos3/plugins/robots/generic/control_interface/ci_tags_actuator.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CTagsDefaultActuator : public CSimulatedActuator,
                                public CCI_TagsActuator {

   public:

      CTagsDefaultActuator();

      virtual ~CTagsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CTagEquippedEntity* m_pcTagEquippedEntity;

   };

}

#endif
