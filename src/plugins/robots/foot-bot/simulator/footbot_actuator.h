/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_ACTUATOR_H
#define FOOTBOT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

namespace argos {

   class CFootBotActuator : public CActuator {

   public:

      CFootBotActuator() :
         m_pcFootBotEntity(NULL) {}
      virtual ~CFootBotActuator() {}

      inline virtual CFootBotEntity& GetEntity() {
         return *m_pcFootBotEntity;
      }
      inline virtual void SetEntity(CEntity& c_entity) {
         m_pcFootBotEntity = dynamic_cast<CFootBotEntity*>(&c_entity);
         if(m_pcFootBotEntity == NULL) {
            THROW_ARGOSEXCEPTION("Cannot associate a foot-bot actuator to a robot of type \"" << c_entity.GetTypeDescription() << "\"");
         }
      }

   private:

      CFootBotEntity* m_pcFootBotEntity;

   };

}

#endif
