/* -*- Mode: C++ -*-
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_actuator.h>
 *
 * @brief This file provides the definition of
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
