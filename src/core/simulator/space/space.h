/**
 * @file <argos3/core/simulator/space/space.h>
 *
 * @brief This file provides the definition of the space.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPACE_H
#define SPACE_H

namespace argos {
   class CSpace;
   class CRay;
   class CFloorEntity;
}

#include <argos3/core/utility/datatypes/any.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/medium_entity.h>
//#include <argos3/core/simulator/entity/led_entity.h>
//#include <argos3/core/simulator/entity/rab_equipped_entity.h>

namespace argos {

   template <typename ACTION>
   class CSpaceOperation : public CEntityOperation<ACTION, CSpace, void> {
   public:
      virtual ~CSpaceOperation() {}
   };

   class CSpaceOperationAddEntity : public CSpaceOperation<CSpaceOperationAddEntity> {
   public:
      virtual ~CSpaceOperationAddEntity() {}
   };
   class CSpaceOperationRemoveEntity : public CSpaceOperation<CSpaceOperationRemoveEntity> {
   public:
      virtual ~CSpaceOperationRemoveEntity() {}
   };

   /****************************************/
   /****************************************/

   class CSpace : public CBaseConfigurableResource {

   public:

      /** Maps for quick access to physical entities (robots, objects) */
      typedef std::map <std::string, CAny, std::less <std::string> > TMapPerType;
      typedef std::map <std::string, TMapPerType, std::less <std::string> > TMapPerTypePerId;

      template <class E> struct SEntityIntersectionItem {
         E* IntersectedEntity;
         Real TOnRay;

         SEntityIntersectionItem() :
            IntersectedEntity(NULL),
            TOnRay(0.0f) {}

         SEntityIntersectionItem(E* pc_entity, Real f_t_on_ray) :
            IntersectedEntity(pc_entity),
            TOnRay(f_t_on_ray) {}

         inline bool operator<(const SEntityIntersectionItem& s_item) {
            return TOnRay < s_item.TOnRay;
         }
      };

      template <class E> struct SEntityIntersectionData {
         bool Intersection;
         std::vector<SEntityIntersectionItem<E>*> IntersectedEntities;

         SEntityIntersectionData() :
            Intersection(false) {}

         SEntityIntersectionData(std::vector<SEntityIntersectionItem<E>*>& c_entities) :
            Intersection(c_entities.size() > 0),
            IntersectedEntities(c_entities) {}
      };

   protected:

      class CRayEmbodiedEntityIntersectionMethod;
      class CRayEmbodiedEntityIntersectionSpaceHash;
      class CRayEmbodiedEntityIntersectionEntitySweep;

      /****************************************/
      /****************************************/

   public:

      CSpace();
      virtual ~CSpace() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      inline UInt32 GetNumberEntities() const {
         return m_vecEntities.size();
      }

      inline CEntity::TVector& GetEntityVector() {
         return m_vecEntities;
      }

      inline CEntity& GetEntity(const std::string& str_id) {
         CEntity::TMap::const_iterator it = m_mapEntitiesPerId.find(str_id);
         if ( it != m_mapEntitiesPerId.end()) {
            return *(it->second);
         }
         THROW_ARGOSEXCEPTION("Unknown entity id \"" << str_id <<
                              "\" when requesting entity from space.");
      }

      void GetEntitiesMatching(CEntity::TVector& t_buffer,
                               const std::string& str_pattern);

      bool GetClosestEmbodiedEntityIntersectedByRay(SEntityIntersectionItem<CEmbodiedEntity>& s_data,
                                                    const CRay& c_ray,
                                                    const TEmbodiedEntitySet& set_ignored_entities = TEmbodiedEntitySet());

      inline CEntity::TMap& GetEntityMapPerId() {
         return m_mapEntitiesPerId;
      }

      inline TMapPerTypePerId& GetEntityMapPerTypePerId() {
         return m_mapEntitiesPerTypePerId;
      }

      TMapPerType& GetEntitiesByType(const std::string& str_type);

      inline CFloorEntity& GetFloorEntity() {
         if(m_pcFloorEntity != NULL) return *m_pcFloorEntity;
         else THROW_ARGOSEXCEPTION("No floor entity has been added to the space.");
      }

      inline void SetFloorEntity(CFloorEntity& c_floor_entity) {
         m_pcFloorEntity = &c_floor_entity;
      }

      inline bool IsUsingSpaceHash() const {
         return m_bUseSpaceHash;
      }

      inline virtual void SetPhysicsEngines(CPhysicsEngine::TVector& t_engines) {
         m_ptPhysicsEngines = &t_engines;
      }

      virtual void Update();

      template <typename ENTITY>
      void AddEntity(ENTITY& c_entity) {
         /* Check that the id of the entity is not already present */
         if(m_mapEntitiesPerId.find(c_entity.GetId()) != m_mapEntitiesPerId.end()) {
            THROW_ARGOSEXCEPTION("Error inserting a " << c_entity.GetTypeDescription() << " entity with id \"" << c_entity.GetId() << "\". An entity with that id exists already.");
         }
         /* Add the entity to the indexes */
         m_vecEntities.push_back(&c_entity);
         m_mapEntitiesPerId[c_entity.GetId()] = &c_entity;
         m_mapEntitiesPerTypePerId[c_entity.GetTypeDescription()][c_entity.GetId()] = &c_entity;
      }

      template <typename ENTITY>
      void RemoveEntity(ENTITY& c_entity) {
         /* Search for entity in the index per type */
         TMapPerTypePerId::iterator itMapPerType = m_mapEntitiesPerTypePerId.find(c_entity.GetTypeDescription());
         if(itMapPerType != m_mapEntitiesPerTypePerId.end()) {
            /* Search for entity in the index per type per id */
            TMapPerType::iterator itMapPerTypePerId = itMapPerType->second.find(c_entity.GetId());
            if(itMapPerTypePerId != itMapPerType->second.end()) {
               /* Get iterators for other indexes */
               CEntity::TVector::iterator itVec = find(m_vecEntities.begin(),
                                                       m_vecEntities.end(),
                                                       &c_entity);
               CEntity::TMap::iterator itMap = m_mapEntitiesPerId.find(c_entity.GetId());
               /* Remove the entity from the indexes */
               m_vecEntities.erase(itVec);
               m_mapEntitiesPerId.erase(itMap);
               itMapPerType->second.erase(itMapPerTypePerId);
               /* Remove entity object */
               c_entity.Destroy();
               delete &c_entity;
               return;
            }
         }
         THROW_ARGOSEXCEPTION("CSpace::RemoveEntity() : Entity \"" <<
                              c_entity.GetId() <<
                              "\" has not been found in the indexes.");
      }

      inline UInt32 GetSimulationClock() const {
         return m_unSimulationClock;
      }

      inline void SetSimulationClock(UInt32 un_simulation_clock) {
         m_unSimulationClock = un_simulation_clock;
      }

      inline void IncreaseSimulationClock(UInt32 un_increase = 1) {
         m_unSimulationClock += un_increase;
      }

      inline const CVector3& GetArenaSize() const {
         return m_cArenaSize;
      }

      inline void SetArenaSize(const CVector3& c_size) {
         m_cArenaSize = c_size;
      }

      inline CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>& GetEmbodiedEntitiesSpaceHash() {
         if(IsUsingSpaceHash()) {
            return *m_pcEmbodiedEntitiesSpaceHash;
         }
         else {
            THROW_ARGOSEXCEPTION("Attempted to access the space hash of embodied entities, but in the XML the user chose not to use it. Maybe you use a sensor or an actuator that references it directly?");
         }
      }

/*
      inline CSpaceHash<CLedEntity, CLEDEntitySpaceHashUpdater>& GetLEDEntitiesSpaceHash() {
         if(IsUsingSpaceHash()) {
            return *m_pcLEDEntitiesSpaceHash;
         }
         else {
            THROW_ARGOSEXCEPTION("Attempted to access the space hash of LED entities, but in the XML the user chose not to use it. Maybe you use a sensor or an actuator that references it directly?");
         }
      }
*/

/*
      inline CSpaceHash<CRABEquippedEntity, CRABEquippedEntitySpaceHashUpdater>& GetRABEquippedEntitiesSpaceHash() {
         if(IsUsingSpaceHash()) {
            return *m_pcRABEquippedEntitiesSpaceHash;
         }
         else {
            THROW_ARGOSEXCEPTION("Attempted to access the space hash of RAB equipped entities, but in the XML the user chose not to use it. Maybe you use a sensor or an actuator that references it directly?");
         }
      }
*/

   protected:

      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);
      virtual void AddMediumEntity(CMediumEntity& c_entity);
      virtual void RemoveMediumEntity(CMediumEntity& c_entity);
      
      inline void UpdateSpaceData() {
         if(IsUsingSpaceHash()) {
            m_pcEmbodiedEntitiesSpaceHash->Update();
//            m_pcLEDEntitiesSpaceHash->Update();
//            m_pcRABEquippedEntitiesSpaceHash->Update();
         }
      }
      
      virtual void UpdateControllableEntities() = 0;
      virtual void UpdatePhysics() = 0;
      
      void UpdateMediumEntities();

      void Distribute(TConfigurationNode& t_tree);

      void AddBoxStrip(TConfigurationNode& t_tree);

      bool GetClosestEmbodiedEntityIntersectedByRaySpaceHash(SEntityIntersectionItem<CEmbodiedEntity>& s_data,
                                                             const CRay& c_ray,
                                                             const TEmbodiedEntitySet& set_ignored_entities);

      bool GetClosestEmbodiedEntityIntersectedByRayEntitySweep(SEntityIntersectionItem<CEmbodiedEntity>& s_data,
                                                               const CRay& c_ray,
                                                               const TEmbodiedEntitySet& set_ignored_entities);

   protected:

      friend class CSpaceOperationAddControllableEntity;
      friend class CSpaceOperationRemoveControllableEntity;
      friend class CSpaceOperationAddMediumEntity;
      friend class CSpaceOperationRemoveMediumEntity;

   protected:

      /** The current simulation clock */
      UInt32 m_unSimulationClock;

      /** Arena size */
      CVector3 m_cArenaSize;

      /** A vector of entities. */
      CEntity::TVector m_vecEntities;

      /** A map of entities. */
      CEntity::TMap m_mapEntitiesPerId;

      /** A map of maps of all the simulated entities.
          The top-level map is indexed by type, as returned by CEntity::GetTypeDescription().
          The second-level maps are indexed by entity id */
      TMapPerTypePerId m_mapEntitiesPerTypePerId;

      /** The space hash of embodied entities */
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>* m_pcEmbodiedEntitiesSpaceHash;

      /** The space hash of LED entities */
//      CSpaceHash<CLedEntity, CLEDEntitySpaceHashUpdater>* m_pcLEDEntitiesSpaceHash;

      /** The space hash of RAB equipped entities */
//      CSpaceHash<CRABEquippedEntity, CRABEquippedEntitySpaceHashUpdater>* m_pcRABEquippedEntitiesSpaceHash;

      /** A vector of controllable entities */
      CControllableEntity::TVector m_vecControllableEntities;

      /** A vector of medium entities */
      CMediumEntity::TVector m_vecMediumEntities;

      /** The floor entity */
      CFloorEntity* m_pcFloorEntity;

      /** True if the space hash should be used */
      bool m_bUseSpaceHash;

      /** Method to calculate the ray-embodied entity intersection */
      CRayEmbodiedEntityIntersectionMethod* m_pcRayEmbodiedEntityIntersectionMethod;

      /** A reference to the list of physics engines */
      CPhysicsEngine::TVector* m_ptPhysicsEngines;
   };

}

#define REGISTER_SPACE_OPERATION(ACTION, OPERATION, ENTITY) \
   REGISTER_ENTITY_OPERATION(ACTION, CSpace, OPERATION, void, ENTITY);

#endif
