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
   class CRay3;
   class CFloorEntity;
   class CSimulator;
}

#include <argos3/core/utility/datatypes/any.h>
#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CSpace : public CBaseConfigurableResource {

   public:

      /**
       * A map of entities indexed by type description.
       * <p>
       * This map type is particularly useful when one wants to execute
       * operations on a specific entity type. For instance, one could want to
       * get the list of all robots of a specific type, and execute operations
       * for that robot.
       * </p>
       * <p>
       * The elements in this map are indexed by type description (the string
       * returned by CEntity::GetTypeDescription()). The elements themselves
       * are stored as objects of the CAny class. This is because any_cast()
       * is faster than <tt>dynamic_cast</tt>, and equally type-safe.
       * </p>
       * @see CSpace::GetEntitiesByType()
       * @see CEntity::GetTypeDescription()
       * @see CAny
       * @see any_cast()
       * @see TMapPerTypePerId
       */
      typedef std::map <std::string, CAny, std::less <std::string> > TMapPerType;

      /**
       * A map of entities indexed by type description and by id.
       * <p>
       * This map type is particularly useful when one wants to execute
       * operations on a specific entity type with a specific id. For instance,
       * one could want to operate on a list of robots with a specific type and id,
       * and execute operations for each robot.
       * </p>
       * <p>
       * The elements in this map are indexed by type description (the string
       * returned by CEntity::GetTypeDescription()), and by id (the string returned by
       * CEntity::GetId()). The elements themselves are stored as objects of the CAny
       * class. This is because any_cast() is faster than <tt>dynamic_cast</tt>, and
       * equally type-safe.
       * </p>
       * @see CSpace::GetEntitiesByType()
       * @see CEntity::GetTypeDescription()
       * @see CEntity::GetId()
       * @see CAny
       * @see any_cast()
       */
      typedef std::map <std::string, TMapPerType, std::less <std::string> > TMapPerTypePerId;

      /****************************************/
      /****************************************/

   public:

      /**
       * Class constructor.
       */
      CSpace();

      /**
       * Class destructor.
       */
      virtual ~CSpace() {}

      /**
       * Initializes the space using the <tt>&lt;arena&gt;</tt> section of the XML configuration file.
       * @param t_tree the <tt>&lt;arena&gt;</tt> section of the XML configuration file.
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Reset the space and all its entities.
       */
      virtual void Reset();

      /**
       * Destroys the space and all its entities.
       */
      virtual void Destroy();

      /**
       * Returns the number of entities contained in the space.
       */
      inline UInt32 GetNumberEntities() const {
         return m_vecEntities.size();
      }

      /**
       * Returns a vector of all the entities in the space.
       * All entities are returned, i.e., all the components of a robot.
       * @return a vector of all the entities in the space.
       * @see GetRootEntityVector()
       */
      inline CEntity::TVector& GetEntityVector() {
         return m_vecEntities;
      }

      /**
       * Returns a vector of all the root entities in the space.
       * A root entity is an entity that has no parent.
       * This method differs from GetEntityVector() in that the latter
       * returns all entities including the components of a composable
       * entity, while this method does not return any component, but only
       * the parentless composables.
       * @return a vector of all the root entities in the space.
       * @see GetEntityVector()
       */
      inline CEntity::TVector& GetRootEntityVector() {
         return m_vecRootEntities;
      }

      /**
       * Returns the entity with the given id.
       * @param str_id The id of the wanted entity
       * @return The entity with the given id.
       * @throws CARGoSException if an entity with the wanted id does not exist
       */
      inline CEntity& GetEntity(const std::string& str_id) {
         CEntity::TMap::const_iterator it = m_mapEntitiesPerId.find(str_id);
         if ( it != m_mapEntitiesPerId.end()) {
            return *(it->second);
         }
         THROW_ARGOSEXCEPTION("Unknown entity id \"" << str_id <<
                              "\" when requesting entity from space.");
      }

      /**
       * Returns the entities matching a given pattern.
       * The pattern must be a valid regexp.
       * @param t_buffer A vector filled with all the entities that match the given pattern.
       * @param str_pattern The pattern to match.
       * @return The entity with the given id.
       * @throws CARGoSException if the regexp is not valid.
       */
      void GetEntitiesMatching(CEntity::TVector& t_buffer,
                               const std::string& str_pattern);

      /**
       * Returns a map of all entities ordered by id.
       * @return a map of all entities ordered by id.
       */
      inline CEntity::TMap& GetEntityMapPerId() {
         return m_mapEntitiesPerId;
      }

      /**
       * Returns a nested map of entities, ordered by type and by id.
       * The 'type' here refers to the string returned by CEntity::GetTypeDescription().
       * Take this example:
       * <code>
       *    CSpace::TMapPerTypePerId& theMap = space.GetEntityMapPerTypePerId();
       *    // theMap["box"] is a CSpace::TMapPerType containing all the box entities, ordered by id
       *    // theMap["led"] is a CSpace::TMapPerType containing all the led entities, ordered by id
       *    // etc.
       *    CBoxEntity* box = any_cast<CBoxEntity*>(theMap["box"]["my_box_22"]);
       *    // do stuff with the box ...
       * </code>
       * @returns a nested map of entities, ordered by type and by id.
       * @see CEntity::GetTypeDescription()
       * @see TMapPerTypePerId
       * @see GetEntitiesByType()
       */
      inline TMapPerTypePerId& GetEntityMapPerTypePerId() {
         return m_mapEntitiesPerTypePerId;
      }

      /**
       * Returns a map containing all the objects of a given type.
       * The 'type' here refers to the string returned by CEntity::GetTypeDescription().
       * Take this example:
       * <code>
       *    CSpace::TMapPerType& theMap = space.GetEntitiesByType("box");
       *    CBoxEntity* box = any_cast<CBoxEntity*>(theMap["my_box"]);
       *    // do stuff with the box ...
       * </code>
       * @param str_type The wanted type to search for.
       * @return A map containing all the objects of a given type.
       * @throws CARGoSException if the given type is not valid.
       * @see CEntity::GetTypeDescription()
       * @see TMapPerType
       * @see GetEntityMapPerTypePerId()
       */

      TMapPerType& GetEntitiesByType(const std::string& str_type) {
        return GetEntitiesByTypeImpl(str_type);
      }

      const TMapPerType& GetEntitiesByType(const std::string& str_type) const {
        return GetEntitiesByTypeImpl(str_type);
      }

      /**
       * Returns the floor entity.
       * @throws CARGoSException if the floor entity has not been added to the arena.
       * @return The floor entity.
       */
      inline CFloorEntity& GetFloorEntity() {
         if(m_pcFloorEntity != NULL) return *m_pcFloorEntity;
         else THROW_ARGOSEXCEPTION("No floor entity has been added to the arena.");
      }

      /**
       * Sets the floor entity.
       * @param c_floor_entity The floor entity.
       */
      inline void SetFloorEntity(CFloorEntity& c_floor_entity) {
         m_pcFloorEntity = &c_floor_entity;
      }

      /**
       * Updates the space.
       * The operations are performed in the following order:
       * <ol>
       * <li>UpdateControllableEntitiesAct()
       * <li>UpdatePhysics()
       * <li>UpdateMedia()
       * <li>UpdateControllableEntitiesSenseStep()
       * </ol>
       * @see UpdateControllableEntitiesAct()
       * @see UpdatePhysics()
       * @see UpdateMedia()
       * @see UpdateControllableEntitiesSenseStep()
       */
      virtual void Update();

      /**
       * Adds an entity of the given type.
       * This method is used internally, don't use it in your code.
       * throws CARGoSException if the entity id already exists in the space indexes.
       */
      template <typename ENTITY>
      void AddEntity(ENTITY& c_entity) {
         std::string strEntityQualifiedName = c_entity.GetContext() + c_entity.GetId();
         /* Check that the id of the entity is not already present */
         if(m_mapEntitiesPerId.find(strEntityQualifiedName) != m_mapEntitiesPerId.end()) {
            THROW_ARGOSEXCEPTION("Error inserting a " <<
                                 c_entity.GetTypeDescription() <<
                                 " entity with id \"" <<
                                 strEntityQualifiedName <<
                                 "\". An entity with that id already exists.");
         }
         /* Add the entity to the indexes */
         if(!c_entity.HasParent()) {
            m_vecRootEntities.push_back(&c_entity);
         }
         /* Calculate index of in the global vector */
         size_t unIdx =
            !m_vecEntities.empty() ?
            m_vecEntities.back()->GetIndex() + 1
            :
            0;
         /* Add entity to global vector */
         m_vecEntities.push_back(&c_entity);
         c_entity.SetIndex(unIdx);
         m_mapEntitiesPerId[strEntityQualifiedName] = &c_entity;
         m_mapEntitiesPerTypePerId[c_entity.GetTypeDescription()][strEntityQualifiedName] = &c_entity;
      }

      /**
       * Removes an entity of the given type.
       * This method is used internally, don't use it in your code.
       * throws CARGoSException if the entity id does not exist in the space indexes.
       */
      template <typename ENTITY>
      void RemoveEntity(ENTITY& c_entity) {
         std::string strEntityQualifiedName = c_entity.GetContext() + c_entity.GetId();
         /* Search for entity in the index per type */
         TMapPerTypePerId::iterator itMapPerType = m_mapEntitiesPerTypePerId.find(c_entity.GetTypeDescription());
         if(itMapPerType != m_mapEntitiesPerTypePerId.end()) {
            /* Search for entity in the index per type per id */
            TMapPerType::iterator itMapPerTypePerId = itMapPerType->second.find(strEntityQualifiedName);
            if(itMapPerTypePerId != itMapPerType->second.end()) {
               /* Remove the entity from the indexes */
               CEntity::TVector::iterator itVec = find(m_vecEntities.begin(),
                                                       m_vecEntities.end(),
                                                       &c_entity);
               m_vecEntities.erase(itVec);
               CEntity::TMap::iterator itMap = m_mapEntitiesPerId.find(strEntityQualifiedName);
               itMapPerType->second.erase(itMapPerTypePerId);
               m_mapEntitiesPerId.erase(itMap);
               if(!c_entity.HasParent()) {
                  CEntity::TVector::iterator itRootVec = find(m_vecRootEntities.begin(),
                                                              m_vecRootEntities.end(),
                                                              &c_entity);
                  m_vecRootEntities.erase(itRootVec);
               }
               /* Remove entity object */
               c_entity.Destroy();
               delete &c_entity;
               return;
            }
         }
         THROW_ARGOSEXCEPTION("CSpace::RemoveEntity() : Entity \"" <<
                              strEntityQualifiedName <<
                              "\" has not been found in the indexes.");
      }

      /**
       * Returns the current value of the simulation clock.
       * The clock is measured in ticks. You can set how much a tick is long in seconds in the XML.
       * @return The current value of the simulation clock.
       */
      inline UInt32 GetSimulationClock() const {
         return m_unSimulationClock;
      }

      /**
       * Sets a new value for the simulation clock.
       * The clock is measured in ticks. You can set how much a tick is long in seconds in the XML.
       * @param un_simulation_clock The new value for the simulation clock.
       */
      inline void SetSimulationClock(UInt32 un_simulation_clock) {
         m_unSimulationClock = un_simulation_clock;
      }

      /**
       * Increases the simulation clock by the wanted value.
       * The clock is measured in ticks. You can set how much a tick is long in seconds in the XML.
       * @param un_increase The quantity to add to the current value of the simulation clock.
       */
      inline void IncreaseSimulationClock(UInt32 un_increase = 1) {
         m_unSimulationClock += un_increase;
      }

      /**
       * Returns the arena size.
       * @return the arena size.
       */
      inline const CVector3& GetArenaSize() const {
         return m_cArenaSize;
      }

      /**
       * Sets the arena size.
       * @return the arena size.
       */
      inline void SetArenaSize(const CVector3& c_size) {
         m_cArenaSize = c_size;
         m_cArenaLimits.Set(m_cArenaCenter - m_cArenaSize,
                            m_cArenaCenter + m_cArenaSize);
      }

      /**
       * Returns the arena center.
       * @return the arena center.
       */
      inline const CVector3& GetArenaCenter() const {
         return m_cArenaCenter;
      }

      /**
       * Sets the arena center.
       * @return the arena center.
       */
      inline void SetArenaCenter(const CVector3& c_center) {
         m_cArenaCenter = c_center;
         m_cArenaLimits.Set(m_cArenaCenter - m_cArenaSize,
                            m_cArenaCenter + m_cArenaSize);
      }

      /*
       * Returns the arena limits.
       * The arena limits are defined by <tt>arena center - arena size</tt> and
       * <tt>arena center - arena size</tt>.
       * @return the arena limits.
       */
      inline const CRange<CVector3>& GetArenaLimits() const {
         return m_cArenaLimits;
      }

      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);
      virtual void AddEntityToPhysicsEngine(CEmbodiedEntity& c_entity);

   protected:

      virtual void UpdateControllableEntitiesAct() = 0;
      virtual void UpdatePhysics() = 0;
      virtual void UpdateMedia() = 0;
      virtual void UpdateControllableEntitiesSenseStep() = 0;

      void Distribute(TConfigurationNode& t_tree);

      void AddBoxStrip(TConfigurationNode& t_tree);

   protected:

      friend class CSpaceOperationAddControllableEntity;
      friend class CSpaceOperationRemoveControllableEntity;
      friend class CSpaceOperationAddEmbodiedEntity;

   protected:

      /* The active simulator instance */
      CSimulator& m_cSimulator;

      /** The current simulation clock */
      UInt32 m_unSimulationClock;

      /** Arena center */
      CVector3 m_cArenaCenter;

      /** Arena size */
      CVector3 m_cArenaSize;

      /** Arena limits */
      CRange<CVector3> m_cArenaLimits;

      /** A vector of entities. */
      CEntity::TVector m_vecEntities;

      /** A vector of all the entities without a parent */
      CEntity::TVector m_vecRootEntities;

      /** A map of entities. */
      CEntity::TMap m_mapEntitiesPerId;

      /** A map of maps of all the simulated entities.
          The top-level map is indexed by type, as returned by CEntity::GetTypeDescription().
          The second-level maps are indexed by entity id */
      TMapPerTypePerId m_mapEntitiesPerTypePerId;

      /** A vector of controllable entities */
      CControllableEntity::TVector m_vecControllableEntities;

      /** The floor entity */
      CFloorEntity* m_pcFloorEntity;

      /** A pointer to the list of physics engines */
      CPhysicsEngine::TVector* m_ptPhysicsEngines;

      /** A pointer to the list of media */
      CMedium::TVector* m_ptMedia;

  private:
      TMapPerType& GetEntitiesByTypeImpl(const std::string& str_type) const;
   };

   /****************************************/
   /****************************************/

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

}

   /****************************************/
   /****************************************/

#define SPACE_OPERATION_ADD_ENTITY(ENTITY)                                 \
   class CSpaceOperationAdd ## ENTITY : public CSpaceOperationAddEntity {  \
   public:                                                                 \
      void ApplyTo(CSpace& c_space, ENTITY& c_entity) {                    \
         c_space.AddEntity(c_entity);                                      \
      }                                                                    \
   };

#define SPACE_OPERATION_REMOVE_ENTITY(ENTITY)                                   \
   class CSpaceOperationRemove ## ENTITY : public CSpaceOperationRemoveEntity { \
   public:                                                                      \
      void ApplyTo(CSpace& c_space, ENTITY& c_entity) {                         \
         c_space.RemoveEntity(c_entity);                                        \
      }                                                                         \
   };

#define REGISTER_SPACE_OPERATION(ACTION, OPERATION, ENTITY)             \
   REGISTER_ENTITY_OPERATION(ACTION, CSpace, OPERATION, void, ENTITY);

#define REGISTER_STANDARD_SPACE_OPERATION_ADD_ENTITY(ENTITY)            \
   SPACE_OPERATION_ADD_ENTITY(ENTITY)                                   \
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,                   \
                            CSpaceOperationAdd ## ENTITY,               \
                            ENTITY);

#define REGISTER_STANDARD_SPACE_OPERATION_REMOVE_ENTITY(ENTITY)         \
   SPACE_OPERATION_REMOVE_ENTITY(ENTITY)                                \
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,                \
                            CSpaceOperationRemove ## ENTITY,            \
                            ENTITY);

#define REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(ENTITY) \
   REGISTER_STANDARD_SPACE_OPERATION_ADD_ENTITY(ENTITY)      \
   REGISTER_STANDARD_SPACE_OPERATION_REMOVE_ENTITY(ENTITY)

#endif
