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
#include <argos3/core/simulator/space/space.h>

namespace argos {
   class CComposableEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      CComposableEntity(CComposableEntity* pc_parent);

      CComposableEntity(CComposableEntity* pc_parent,
                        const std::string& str_id);

      virtual ~CComposableEntity();

      virtual void Reset();

      virtual void Destroy();

      virtual void Update();

      virtual std::string GetTypeDescription() const {
         return "composite";
      }

      virtual void UpdateComponents();

      void AddComponent(CEntity& c_component);

      CEntity& RemoveComponent(const std::string& str_component);

      CEntity& GetComponent(const std::string& str_component);

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

      bool HasComponent(const std::string& str_component);

      CEntity::TMultiMap::iterator FindComponent(const std::string& str_component);

      inline CEntity::TMultiMap& GetComponents() {
         return m_mapComponents;
      }

   private:

      CEntity::TMultiMap m_mapComponents;

   };

   class CSpaceOperationAddComposableEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CComposableEntity& c_entity) {
         c_space.AddEntity(c_entity);
         for(CEntity::TMultiMap::iterator it = c_entity.GetComponents().begin();
             it != c_entity.GetComponents().end();
             ++it) {
            CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(c_space, *(it->second));
         }
      }
   };
   
   class CSpaceOperationRemoveComposableEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CComposableEntity& c_entity) {
         for(CEntity::TMultiMap::iterator it = c_entity.GetComponents().begin();
             it != c_entity.GetComponents().end();
             ++it) {
            CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(c_space, *(it->second));
         }
         c_space.RemoveEntity(c_entity);
      }
   };

}

#endif
