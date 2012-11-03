/**
 * @file simulator/entity/composable_entity.h
 *
 * @brief This file contains the definition of an entity.
 *
 * This file contains the class definition of an
 * entity, that is, the basic class that provides the interface for the
 * simulation of all the objects in the environment.
 *
 * @author Carlo Pinciroli - <cpinciroli@ulb.ac.be>
 */

#ifndef COMPOSABLE_ENTITY_H
#define COMPOSABLE_ENTITY_H

namespace argos {
   class CComposableEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {
   class CComposableEntity : public CEntity {

   public:

      CComposableEntity(CEntity* pc_parent) :
         CEntity(pc_parent) {}
      virtual ~CComposableEntity() {}

      virtual CEntity& GetComponent(const std::string& str_component) = 0;

      template <class E> E& GetComponent(const std::string& str_component) {
         E* pcComponent = dynamic_cast<E*>(&GetComponent(str_component));
         if(pcComponent != NULL) {
            return *pcComponent;
         }
         else {
            THROW_ARGOSEXCEPTION("Entity \"" <<
                                 GetId() <<
                                 "\" does not have component of type");
         }
      }

      virtual bool HasComponent(const std::string& str_component) = 0;

      inline virtual std::string GetTypeDescription() const {
         return "composable_entity";
      }

      virtual void UpdateComponents() = 0;
   };
}

#endif
