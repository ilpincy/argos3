/**
 * @file <argos3/core/simulator/entity/composable_entity.h>
 *
 * @brief This file contains the definition of an entity.
 *
 * This file contains the class definition of an
 * entity, that is, the basic class that provides the interface for the
 * simulation of all the objects in the environment.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
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

      virtual void AddComponent(CEntity& c_component);

      virtual void RemoveComponent(const std::string& str_component);

      virtual CEntity& GetComponent(const std::string& str_component);

      template <class E>
      E& GetComponent(const std::string& str_component) {
         E* pcComponent = dynamic_cast<E*>(&GetComponent(str_component));
         if(pcComponent != NULL) {
            return *pcComponent;
         }
         else {
            THROW_ARGOSEXCEPTION("Type conversion failed for component type \"" << str_component << "\" of entity \"" << GetId());
         }
      }

      virtual bool HasComponent(const std::string& str_component);

      virtual void Update();

   protected:

      CEntity::TMultiMap::iterator FindComponent(const std::string& str_component);

      virtual void UpdateComponents();

   private:

      CEntity::TMultiMap m_mapComponents;

   };
}

#endif
