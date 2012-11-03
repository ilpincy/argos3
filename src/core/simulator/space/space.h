/**
 * @file core/simulator/space/space.h
 *
 * @brief This file provides the definition of the space.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SPACE_H
#define SPACE_H

namespace argos {
   class CSpace;
   class CRay;
   class CFloorEntity;
}

#include <argos3/core/utility/datatypes/any.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/medium_entity.h>
//#include <argos3/core/simulator/entity/led_entity.h>
//#include <argos3/core/simulator/entity/rab_equipped_entity.h>

namespace argos {

   class CSpace : public CBaseConfigurableResource {

      /****************************************/
      /****************************************/

   public:

      /** Maps for quick access to physical entities (robots, objects) */
      typedef std::map <std::string, CAny, std::less <std::string> > TAnyEntityMap;
      typedef std::map <std::string, TAnyEntityMap, std::less <std::string> > TMapOfAnyEntityMaps;

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
         CEntity::TMap::const_iterator it = m_mapEntities.find(str_id);
         if ( it != m_mapEntities.end()) {
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

      inline CEntity::TMap& GetAllEntities() {
         return m_mapEntities;
      }

      inline TMapOfAnyEntityMaps& GetEntitiesMapOfMaps() {
         return m_mapOfMapsEntities;
      }

      inline TAnyEntityMap& GetEntitiesByType(const std::string str_type) {
         TMapOfAnyEntityMaps::iterator itEntities = m_mapOfMapsEntities.find(str_type);
         if (itEntities != m_mapOfMapsEntities.end()){
            return itEntities->second;
         }
         THROW_ARGOSEXCEPTION("Entity map for type \"" << str_type << "\" not found.");
      }

      inline CFloorEntity& GetFloorEntity() {
         if(m_pcFloorEntity == NULL) {
            THROW_ARGOSEXCEPTION("No floor entity has been added to the space.");
         }
         return *m_pcFloorEntity;
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

      virtual void AddEntity(CEntity& c_entity);
      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void AddMediumEntity(CMediumEntity& c_entity);

      virtual void RemoveEntity(CEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveMediumEntity(CMediumEntity& c_entity);

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

      /** The current simulation clock */
      UInt32 m_unSimulationClock;

      /** Arena size */
      CVector3 m_cArenaSize;

      /** A vector of entities. */
      CEntity::TVector m_vecEntities;

      /** A map of entities. */
      CEntity::TMap m_mapEntities;

      /** A map of maps of all the simulated entities.
          The map stores several submaps with the key equals to the
          type description of an entity. The submaps store the entities
          according to their id */
      TMapOfAnyEntityMaps m_mapOfMapsEntities;

      /** The space hash of embodied entities */
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>* m_pcEmbodiedEntitiesSpaceHash;

      /** The space hash of LED entities */
//      CSpaceHash<CLedEntity, CLEDEntitySpaceHashUpdater>* m_pcLEDEntitiesSpaceHash;

      /** The space hash of RAB equipped entities */
//      CSpaceHash<CRABEquippedEntity, CRABEquippedEntitySpaceHashUpdater>* m_pcRABEquippedEntitiesSpaceHash;

      /** A vector of controllable entities */
      TControllableEntityVector m_vecControllableEntities;

      /** A vector of medium entities */
      TMediumEntityVector m_vecMediumEntities;

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

#endif
